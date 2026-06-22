/*
  ELEVADOR 4 PISOS — SIMULAÇÃO WOKWI
  - Motor: A4988 + wokwi-stepper-motor (STEP/DIR)
  - Displays: 4x OLED board-ssd1306 (I2C) via TCA9548A
  - Sem LCD

  Requisitos no Wokwi (circuito):
  - 1x TCA9548A (I2C multiplexer) endereço típico 0x70
  - 4x board-ssd1306 (todos podem estar em 0x3C)
  - SDA/SCL do Arduino -> SDA/SCL do TCA9548A
  - Cada OLED ligado ao canal do TCA (SC0/SD0, SC1/SD1, SC2/SD2, SC3/SD3)

  Arduino UNO:
  - I2C: SDA=A4, SCL=A5

  A4988:
  - STEP -> D9
  - DIR  -> D10
  - EN   -> (NÃO USAR A4 POR causa do I2C). Por defeito: sem EN (PIN_EN=-1)
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <stdio.h>

// ================= PINAGEM =================

// --- A4988 (STEP/DIR) ---
#define PIN_STEP 9
#define PIN_DIR  10
#define PIN_EN   (-1)     // não usar (A4 é SDA no UNO)

// Velocidades (passos por segundo) -> ajusta na simulação
#define velocidadeSobe   800
#define velocidadeDesce  650

// --- Botões de chamada ---
#define pinBotaoA 2
#define pinBotaoB 3
#define pinBotaoC 4
#define pinBotaoD 5

// --- RESET ---
#define pinReset 7   // INPUT_PULLUP

// --- Sensores de piso (ativos a LOW) ---
#define pinSensorA 8
#define pinSensorB 11
#define pinSensorC 12
#define pinSensorD 13

// --- Buzzer ---
#define pinBuzzer 6

// --- LEDs de pedidos ---
#define pinLedA A0
#define pinLedB A1
#define pinLedC A2
#define pinLedD A3

// ================= OLED SSD1306 =================
#define OLED_W 128
#define OLED_H 64
#define OLED_ADDR 0x3C    // típico do board-ssd1306
#define TCA_ADDR  0x70    // típico TCA9548A

// 4 instâncias (uma por “canal” do TCA)
Adafruit_SSD1306 oled1(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled2(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled3(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled4(OLED_W, OLED_H, &Wire, -1);

// ================================================================

int ledAndar[4] = { pinLedA, pinLedB, pinLedC, pinLedD };

// ---------------- LÓGICA ELEVADOR ----------------

// Pedidos
bool pedidos[4] = { false, false, false, false };

// sensores (true = ativo)
bool estadoSensorA, estadoSensorB, estadoSensorC, estadoSensorD;
bool estadoSensorAAnt, estadoSensorBAnt, estadoSensorCAnt, estadoSensorDAnt;

int acaoElevador = 0;     // 0=parado, 1=subir, 2=descer
int andar = -1;           // 1..4 ou -1
int andarDestino = -1;    // 1..4
int sentidoAtual = 0;     // 1=subir, -1=descer, 0=indefinido

// --- A4988 stepper runtime ---
volatile int stepRate = 0;           // passos por segundo
unsigned long lastStepMicros = 0;
int motorDir = 0;                    // +1 subir, -1 descer, 0 parado
bool stepLevel = LOW;                // para gerar pulsos STEP

// --- Buzzer curto (não-bloqueante) ---
bool beepAtivo = false;
unsigned long beepAteMs = 0;

// cadências
unsigned long lastLogicMs = 0;

// --- ESTABILIZAÇÃO EM PISO (300ms) ---
const unsigned long TEMPO_ESTABILIZA_MS = 300;
bool estabilizando = false;
unsigned long estabilizaAteMs = 0;

void iniciaEstabilizacao() {
  estabilizando = true;
  estabilizaAteMs = millis() + TEMPO_ESTABILIZA_MS;
  stepRate = 0;
  motorDir = 0;
  if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH); // disable (comum no A4988)
  digitalWrite(PIN_STEP, LOW);
}

// --- DEBOUNCE BOTÕES (50ms) ---
const unsigned long DEBOUNCE_MS = 50;
const int botoes[4] = { pinBotaoA, pinBotaoB, pinBotaoC, pinBotaoD };

bool botaoRawAnt[4] = { true, true, true, true };
bool botaoEstavel[4] = { true, true, true, true };
unsigned long botaoUltMudancaMs[4] = { 0, 0, 0, 0 };

void debounceBotoesERegistaPedidos() {
  unsigned long agora = millis();

  for (int i = 0; i < 4; i++) {
    bool raw = digitalRead(botoes[i]); // HIGH 0, LOW 1

    if (raw != botaoRawAnt[i]) {
      botaoRawAnt[i] = raw;
      botaoUltMudancaMs[i] = agora;
    }

    if ((agora - botaoUltMudancaMs[i]) >= DEBOUNCE_MS) {
      if (raw != botaoEstavel[i]) {
        bool antigo = botaoEstavel[i];
        botaoEstavel[i] = raw;

        // flanco HIGH->LOW => premido
        if (antigo == HIGH && botaoEstavel[i] == LOW) pedidos[i] = true;
      }
    }
  }
}

// --- DEBOUNCE RESET ---
bool resetRawAnt = true;
bool resetEstavel = true;
unsigned long resetUltMudancaMs = 0;

bool resetFoiPremido() {
  unsigned long agora = millis();
  bool raw = digitalRead(pinReset);

  if (raw != resetRawAnt) {
    resetRawAnt = raw;
    resetUltMudancaMs = agora;
  }

  if ((agora - resetUltMudancaMs) >= DEBOUNCE_MS) {
    if (raw != resetEstavel) {
      bool antigo = resetEstavel;
      resetEstavel = raw;
      if (antigo == HIGH && resetEstavel == LOW) return true;
    }
  }
  return false;
}

// ======= STEP PER SERVICE (A4988) =======
void stepperService() {
  if (stepRate <= 0 || motorDir == 0) return;

  digitalWrite(PIN_DIR, (motorDir > 0) ? HIGH : LOW);
  if (PIN_EN != -1) digitalWrite(PIN_EN, LOW); // enable

  unsigned long halfPeriod = 1000000UL / (unsigned long)(2UL * (unsigned long)stepRate);
  unsigned long now = micros();

  if (now - lastStepMicros >= halfPeriod) {
    lastStepMicros = now;
    stepLevel = !stepLevel;
    digitalWrite(PIN_STEP, stepLevel);
  }
}

void beepService() {
  if (beepAtivo && millis() >= beepAteMs) {
    noTone(pinBuzzer);
    beepAtivo = false;
  }
}

// ---------------- TIMEOUT + ERRO + REARME ----------------
enum EstadoSistema { NORMAL, ERRO_MOVENDO, ERRO_PARADO, REARME_ATIVO };
EstadoSistema estadoSistema = NORMAL;

const unsigned long TIMEOUT_MOV_MS = 15000UL;
const unsigned long ERRO_PORTA_MS  = 120000UL;

bool alarmeAtivo = false;
unsigned long inicioMovMs = 0;
bool movimentoAtivo = false;

unsigned long erroPortaAteMs = 0;

enum FaseRearme { REARME_NONE, REARME_ATE_PISO_VALIDO, REARME_PAUSA_2S, REARME_IR_PISO1, REARME_ESPERA_10S };
FaseRearme faseRearme = REARME_NONE;

unsigned long faseAteMs = 0;
unsigned long rearmeMoveStartMs = 0;

void iniciaAlarmeContinuo() {
  if (alarmeAtivo) return;
  beepAtivo = false;
  tone(pinBuzzer, 900);
  alarmeAtivo = true;
}

void desligaAlarmeContinuo() {
  if (!alarmeAtivo) return;
  noTone(pinBuzzer);
  alarmeAtivo = false;
  beepAtivo = false;
}

void beepCurto() {
  if (alarmeAtivo) return;
  tone(pinBuzzer, 800);
  beepAtivo = true;
  beepAteMs = millis() + 120;
}

void limpaPedidosEDesligaLEDs() {
  for (int i = 0; i < 4; i++) pedidos[i] = false;
  for (int i = 0; i < 4; i++) digitalWrite(ledAndar[i], LOW);
}

void entraErroMovendo() {
  estadoSistema = ERRO_MOVENDO;
  iniciaAlarmeContinuo();
  limpaPedidosEDesligaLEDs();
  estabilizando = false;

  stepRate = 0;
  motorDir = 0;
  if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
  digitalWrite(PIN_STEP, LOW);
}

void fixaErroParado() {
  estadoSistema = ERRO_PARADO;

  stepRate = 0;
  motorDir = 0;
  if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
  digitalWrite(PIN_STEP, LOW);

  acaoElevador = 0;
  erroPortaAteMs = millis() + ERRO_PORTA_MS;
}

void iniciaRearme() {
  estadoSistema = REARME_ATIVO;
  limpaPedidosEDesligaLEDs();
  iniciaAlarmeContinuo();
  estabilizando = false;

  if (acaoElevador == 1 || acaoElevador == 2) {
    faseRearme = REARME_ATE_PISO_VALIDO;
  } else {
    faseRearme = REARME_PAUSA_2S;
    faseAteMs = millis() + 2000UL;

    stepRate = 0;
    motorDir = 0;
    if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
    digitalWrite(PIN_STEP, LOW);

    acaoElevador = 0;
  }
}

// --- helpers destino ---
bool haPedidosPendentes() {
  for (int i = 0; i < 4; i++) if (pedidos[i]) return true;
  return false;
}

int proximoAcima(int andarAtual) {
  for (int i = andarAtual; i < 4; i++) if (pedidos[i]) return i + 1;
  return -1;
}

int proximoAbaixo(int andarAtual) {
  for (int i = andarAtual - 2; i >= 0; i--) if (pedidos[i]) return i + 1;
  return -1;
}

void selecionaDestinoEAcao() {
  if (andar < 1 || andar > 4) return;

  if (!haPedidosPendentes()) {
    andarDestino = andar;
    acaoElevador = 0;
    return;
  }

  int dest = -1;

  if (sentidoAtual == 1) {
    dest = proximoAcima(andar);
    if (dest == -1) { dest = proximoAbaixo(andar); if (dest != -1) sentidoAtual = -1; }
  } else if (sentidoAtual == -1) {
    dest = proximoAbaixo(andar);
    if (dest == -1) { dest = proximoAcima(andar); if (dest != -1) sentidoAtual = 1; }
  } else {
    dest = proximoAcima(andar);
    if (dest == -1) dest = proximoAbaixo(andar);
    if (dest != -1) sentidoAtual = (dest > andar) ? 1 : -1;
  }

  if (dest == -1) dest = andar;
  andarDestino = dest;

  if (andarDestino == andar) acaoElevador = 0;
  else if (andarDestino > andar) { acaoElevador = 1; sentidoAtual = 1; }
  else { acaoElevador = 2; sentidoAtual = -1; }
}

void processaChegadaPiso(int piso, int idxPedido) {
  andar = piso;

  if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_ATE_PISO_VALIDO) {
      stepRate = 0; motorDir = 0;
      if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
      digitalWrite(PIN_STEP, LOW);
      acaoElevador = 0;

      faseRearme = REARME_PAUSA_2S;
      faseAteMs = millis() + 2000UL;
      return;
    }
    if (faseRearme == REARME_IR_PISO1) {
      if (piso == 1) {
        stepRate = 0; motorDir = 0;
        if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
        digitalWrite(PIN_STEP, LOW);
        acaoElevador = 0;

        faseRearme = REARME_ESPERA_10S;
        faseAteMs = millis() + 10000UL;
      }
      return;
    }
    return;
  }

  if (estadoSistema == ERRO_MOVENDO) { fixaErroParado(); return; }
  if (estadoSistema == ERRO_PARADO) return;

  bool tinhaPedido = pedidos[idxPedido];
  if (tinhaPedido) beepCurto();
  pedidos[idxPedido] = false;

  if (tinhaPedido || andarDestino == piso) iniciaEstabilizacao();
}

// ===================== OLED x4 via TCA9548A =====================
unsigned long lastOledMs = 0;
bool pedidosAntOLED[4] = { false, false, false, false };
int pedidoRecenteOLED = -1; // 1..4 ou -1

void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

char dirSetaOLED() {
  if (acaoElevador == 1) return '^';
  if (acaoElevador == 2) return 'v';
  return '-';
}

void oledDetetaPedidoRecente() {
  for (int i = 0; i < 4; i++) {
    if (!pedidosAntOLED[i] && pedidos[i]) pedidoRecenteOLED = i + 1;
    pedidosAntOLED[i] = pedidos[i];
  }
}

void renderOneOLED(Adafruit_SSD1306 &d, int idx) {
  d.clearDisplay();
  d.setTextColor(SSD1306_WHITE);

  d.setTextSize(1);
  d.setCursor(0, 0);
  d.print("OLED #");
  d.print(idx);

  d.setTextSize(2);
  d.setCursor(0, 12);
  d.print("P:");
  if (andar >= 1 && andar <= 4) d.print(andar);
  else d.print("?");

  d.setCursor(64, 12);
  d.print(dirSetaOLED());

  d.setTextSize(1);
  d.setCursor(0, 34);
  d.print("Recente: ");
  if (pedidoRecenteOLED >= 1 && pedidoRecenteOLED <= 4) d.print(pedidoRecenteOLED);
  else d.print("-");

  d.setCursor(0, 46);
  d.print("Fila: ");

  int x = 30, y = 46;
  int cont = 0;

  auto printPiso = [&](int p) {
    d.setCursor(x, y);
    d.print(p);
    d.print(' ');
    x += 12;
    cont++;
    if (x > 120) { x = 0; y += 10; }
  };

  if (andar >= 1 && andar <= 4) {
    if (sentidoAtual == 1) {
      for (int p = andar; p <= 4; p++) if (pedidos[p - 1]) printPiso(p);
      for (int p = andar - 1; p >= 1; p--) if (pedidos[p - 1]) printPiso(p);
    } else if (sentidoAtual == -1) {
      for (int p = andar; p >= 1; p--) if (pedidos[p - 1]) printPiso(p);
      for (int p = andar + 1; p <= 4; p++) if (pedidos[p - 1]) printPiso(p);
    } else {
      for (int p = 1; p <= 4; p++) if (pedidos[p - 1]) printPiso(p);
    }
  } else {
    for (int p = 1; p <= 4; p++) if (pedidos[p - 1]) printPiso(p);
  }

  if (cont == 0) {
    d.setCursor(30, 46);
    d.print("-");
  }

  d.setCursor(0, 58);
  d.print("Estado:");
  if (estadoSistema == NORMAL) d.print("NORMAL");
  else if (estadoSistema == ERRO_MOVENDO) d.print("ERRO_MOV");
  else if (estadoSistema == ERRO_PARADO) d.print("ERRO_PAR");
  else d.print("REARME");

  d.display();
}

void oledRenderAll4() {
  // canal 0 -> oled1
  tcaSelect(0); renderOneOLED(oled1, 1);
  // canal 1 -> oled2
  tcaSelect(1); renderOneOLED(oled2, 2);
  // canal 2 -> oled3
  tcaSelect(2); renderOneOLED(oled3, 3);
  // canal 3 -> oled4
  tcaSelect(3); renderOneOLED(oled4, 4);
}

// ===================== SETUP =====================
void setup() {
  pinMode(pinBotaoA, INPUT_PULLUP);
  pinMode(pinBotaoB, INPUT_PULLUP);
  pinMode(pinBotaoC, INPUT_PULLUP);
  pinMode(pinBotaoD, INPUT_PULLUP);

  pinMode(pinReset, INPUT_PULLUP);

  pinMode(pinLedA, OUTPUT);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinLedC, OUTPUT);
  pinMode(pinLedD, OUTPUT);

  pinMode(pinSensorA, INPUT_PULLUP);
  pinMode(pinSensorB, INPUT_PULLUP);
  pinMode(pinSensorC, INPUT_PULLUP);
  pinMode(pinSensorD, INPUT_PULLUP);

  pinMode(pinBuzzer, OUTPUT);

  // A4988
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  if (PIN_EN != -1) pinMode(PIN_EN, OUTPUT);

  digitalWrite(PIN_STEP, LOW);
  digitalWrite(PIN_DIR, LOW);
  if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);

  // I2C + OLEDs via TCA
  Wire.begin();

  // init cada OLED no seu canal
  tcaSelect(0); oled1.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  tcaSelect(1); oled2.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  tcaSelect(2); oled3.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  tcaSelect(3); oled4.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

  // limpa todos
  tcaSelect(0); oled1.clearDisplay(); oled1.display();
  tcaSelect(1); oled2.clearDisplay(); oled2.display();
  tcaSelect(2); oled3.clearDisplay(); oled3.display();
  tcaSelect(3); oled4.clearDisplay(); oled4.display();

  // identifica piso inicial
  estadoSensorA = !digitalRead(pinSensorA);
  estadoSensorB = !digitalRead(pinSensorB);
  estadoSensorC = !digitalRead(pinSensorC);
  estadoSensorD = !digitalRead(pinSensorD);

  andar = -1;
  if (estadoSensorA) andar = 1;
  if (estadoSensorB) andar = 2;
  if (estadoSensorC) andar = 3;
  if (estadoSensorD) andar = 4;

  // se não sabe onde está: desce até encontrar um sensor
  if (andar == -1) {
    motorDir = -1;
    stepRate = velocidadeDesce;

    while (andar == -1) {
      stepperService();
      beepService();

      estadoSensorA = !digitalRead(pinSensorA);
      estadoSensorB = !digitalRead(pinSensorB);
      estadoSensorC = !digitalRead(pinSensorC);
      estadoSensorD = !digitalRead(pinSensorD);

      if (estadoSensorA) andar = 1;
      if (estadoSensorB) andar = 2;
      if (estadoSensorC) andar = 3;
      if (estadoSensorD) andar = 4;
    }

    stepRate = 0;
    motorDir = 0;
    if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
    digitalWrite(PIN_STEP, LOW);
  }

  andarDestino = andar;
  acaoElevador = 0;
  sentidoAtual = 0;

  estadoSensorAAnt = estadoSensorA;
  estadoSensorBAnt = estadoSensorB;
  estadoSensorCAnt = estadoSensorC;
  estadoSensorDAnt = estadoSensorD;

  unsigned long agora = millis();
  for (int i = 0; i < 4; i++) {
    bool raw = digitalRead(botoes[i]);
    botaoRawAnt[i] = raw;
    botaoEstavel[i] = raw;
    botaoUltMudancaMs[i] = agora;
    pedidosAntOLED[i] = pedidos[i];
  }

  resetRawAnt = digitalRead(pinReset);
  resetEstavel = resetRawAnt;
  resetUltMudancaMs = agora;

  pedidoRecenteOLED = -1;
  lastOledMs = 0;
  oledRenderAll4();
}

// ===================== LOOP =====================
void loop() {
  stepperService();
  beepService();

  if (millis() - lastLogicMs < 10) return;
  lastLogicMs = millis();

  if (resetFoiPremido()) iniciaRearme();

  estadoSensorA = !digitalRead(pinSensorA);
  estadoSensorB = !digitalRead(pinSensorB);
  estadoSensorC = !digitalRead(pinSensorC);
  estadoSensorD = !digitalRead(pinSensorD);

  if (estadoSistema == NORMAL) debounceBotoesERegistaPedidos();

  if (estadoSensorA && !estadoSensorAAnt) processaChegadaPiso(1, 0);
  if (estadoSensorB && !estadoSensorBAnt) processaChegadaPiso(2, 1);
  if (estadoSensorC && !estadoSensorCAnt) processaChegadaPiso(3, 2);
  if (estadoSensorD && !estadoSensorDAnt) processaChegadaPiso(4, 3);

  if (estabilizando && millis() >= estabilizaAteMs) estabilizando = false;

  if (estadoSistema == NORMAL) {
    if (!estabilizando && stepRate == 0) selecionaDestinoEAcao();

    bool movimentoCmd = (!estabilizando && (acaoElevador == 1 || acaoElevador == 2));

    if (movimentoCmd && !movimentoAtivo) {
      movimentoAtivo = true;
      inicioMovMs = millis();
    } else if (!movimentoCmd) {
      movimentoAtivo = false;
    }

    if (movimentoAtivo && (millis() - inicioMovMs >= TIMEOUT_MOV_MS)) {
      entraErroMovendo();
      movimentoAtivo = false;
    }
  }

  if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_PAUSA_2S && millis() >= faseAteMs) {
      if (andar == 1) {
        faseRearme = REARME_ESPERA_10S;
        faseAteMs = millis() + 10000UL;
      } else {
        faseRearme = REARME_IR_PISO1;
        rearmeMoveStartMs = millis();
        andarDestino = 1;
        acaoElevador = 2;
        sentidoAtual = -1;
      }
    }

    if (faseRearme == REARME_IR_PISO1) {
      if (millis() - rearmeMoveStartMs >= TIMEOUT_MOV_MS) {
        faseRearme = REARME_NONE;
        entraErroMovendo();
      }
    }

    if (faseRearme == REARME_ESPERA_10S && millis() >= faseAteMs) {
      desligaAlarmeContinuo();
      estadoSistema = NORMAL;
      faseRearme = REARME_NONE;

      for (int i = 0; i < 4; i++) pedidos[i] = false;
      acaoElevador = 0;

      stepRate = 0;
      motorDir = 0;
      if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
      digitalWrite(PIN_STEP, LOW);

      andarDestino = andar;
      sentidoAtual = 0;
    }
  }

  // Comando final do motor
  if (estadoSistema == ERRO_PARADO) {
    stepRate = 0;
    motorDir = 0;
    if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
    digitalWrite(PIN_STEP, LOW);

  } else if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_PAUSA_2S || faseRearme == REARME_ESPERA_10S) {
      stepRate = 0;
      motorDir = 0;
      if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
      digitalWrite(PIN_STEP, LOW);
    } else {
      if (acaoElevador == 0) { stepRate = 0; motorDir = 0; if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH); digitalWrite(PIN_STEP, LOW); }
      if (acaoElevador == 1) { stepRate = velocidadeSobe;  motorDir = +1; }
      if (acaoElevador == 2) { stepRate = velocidadeDesce; motorDir = -1; }
    }

  } else if (estabilizando) {
    stepRate = 0;
    motorDir = 0;
    if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
    digitalWrite(PIN_STEP, LOW);

  } else {
    if (acaoElevador == 0) { stepRate = 0; motorDir = 0; if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH); digitalWrite(PIN_STEP, LOW); }
    if (acaoElevador == 1) { stepRate = velocidadeSobe;  motorDir = +1; }
    if (acaoElevador == 2) { stepRate = velocidadeDesce; motorDir = -1; }
  }

  // LEDs
  if (estadoSistema == NORMAL) {
    for (int i = 0; i < 4; i++) digitalWrite(ledAndar[i], pedidos[i] ? HIGH : LOW);
  } else {
    for (int i = 0; i < 4; i++) digitalWrite(ledAndar[i], LOW);
  }

  // OLEDs
  oledDetetaPedidoRecente();
  if (millis() - lastOledMs >= 100) {
    lastOledMs = millis();
    oledRenderAll4();
  }

  estadoSensorAAnt = estadoSensorA;
  estadoSensorBAnt = estadoSensorB;
  estadoSensorCAnt = estadoSensorC;
  estadoSensorDAnt = estadoSensorD;
}
