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

  Arduino Mega:
  - I2C: SDA=20, SCL=21

  A4988:
  - STEP -> D9
  - DIR  -> D10
  - EN   -> não usado

  Porta:
  - Interruptor deslizante de permissiva -> D22
  - D22 = HIGH -> porta fechada / permissiva válida
  - D22 = LOW  -> porta aberta / movimento bloqueado

  Botões exteriores:
  - Piso 1 -> D2
  - Piso 2 -> D3
  - Piso 3 -> D4
  - Piso 4 -> D5

  Botões interiores da cabine:
  - Piso 1 -> D23
  - Piso 2 -> D24
  - Piso 3 -> D25
  - Piso 4 -> D26

  LEDs exteriores:
  - Piso 1 -> A0
  - Piso 2 -> A1
  - Piso 3 -> A2
  - Piso 4 -> A3

  LEDs interiores da cabine:
  - Piso 1 -> D27
  - Piso 2 -> D28
  - Piso 3 -> D29
  - Piso 4 -> D30

  Funcionamento:
  - Os pedidos exteriores e interiores atuam sobre a MESMA fila de pedidos.
  - Se o piso 3 for pedido por fora ou por dentro:
      -> pedido[2] = true
      -> acende LED exterior piso 3
      -> acende LED interior piso 3
  - Quando o piso é atendido:
      -> pedido apaga
      -> apagam ambos os LEDs desse piso

  Botões:
  - Todos os botões usam INPUT_PULLUP
  - Ligar: pino -> botão -> GND

  LEDs:
  - Cada LED deve ter resistência em série (ex.: 220 ohm)
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
#define PIN_EN   (-1)

// Velocidades (passos por segundo)
#define velocidadeSobe   800
#define velocidadeDesce  650

// --- Botões EXTERIORES ---
#define pinBotaoExtA 2
#define pinBotaoExtB 3
#define pinBotaoExtC 4
#define pinBotaoExtD 5

// --- Buzzer ---
#define pinBuzzer 6

// --- RESET ---
#define pinReset 7   // INPUT_PULLUP

// --- Sensores de piso (ativos a LOW) ---
#define pinSensorA 8
#define pinSensorB 11
#define pinSensorC 12
#define pinSensorD 13

// --- Porta / permissiva ---
#define pinPortaPermissiva 22

// --- Botões INTERIORES da cabine ---
#define pinBotaoIntA 23
#define pinBotaoIntB 24
#define pinBotaoIntC 25
#define pinBotaoIntD 26

// --- LEDs INTERIORES da cabine ---
#define pinLedIntA 27
#define pinLedIntB 28
#define pinLedIntC 29
#define pinLedIntD 30

// --- LEDs EXTERIORES ---
#define pinLedExtA A0
#define pinLedExtB A1
#define pinLedExtC A2
#define pinLedExtD A3

// ================= OLED SSD1306 =================
#define OLED_W 128
#define OLED_H 64
#define OLED_ADDR 0x3C
#define TCA_ADDR  0x70

Adafruit_SSD1306 oled1(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled2(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled3(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled4(OLED_W, OLED_H, &Wire, -1);

// ================================================================

// LEDs exteriores e interiores por piso
int ledsExt[4] = { pinLedExtA, pinLedExtB, pinLedExtC, pinLedExtD };
int ledsInt[4] = { pinLedIntA, pinLedIntB, pinLedIntC, pinLedIntD };

// ---------------- LÓGICA ELEVADOR ----------------

// Pedidos por piso (comuns ao exterior e interior)
bool pedidos[4] = { false, false, false, false };

// sensores (true = ativo)
bool estadoSensorA, estadoSensorB, estadoSensorC, estadoSensorD;
bool estadoSensorAAnt, estadoSensorBAnt, estadoSensorCAnt, estadoSensorDAnt;

int acaoElevador = 0;     // 0=parado, 1=subir, 2=descer
int andar = -1;           // 1..4 ou -1
int andarDestino = -1;    // 1..4
int sentidoAtual = 0;     // 1=subir, -1=descer, 0=indefinido

// --- A4988 runtime ---
volatile int stepRate = 0;
unsigned long lastStepMicros = 0;
int motorDir = 0;
bool stepLevel = LOW;

// --- Buzzer curto (não-bloqueante) ---
bool beepAtivo = false;
unsigned long beepAteMs = 0;

// cadências
unsigned long lastLogicMs = 0;

// --- ESTABILIZAÇÃO EM PISO (300 ms) ---
const unsigned long TEMPO_ESTABILIZA_MS = 300;
bool estabilizando = false;
unsigned long estabilizaAteMs = 0;

void paraMotor() {
  stepRate = 0;
  motorDir = 0;
  if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);
  digitalWrite(PIN_STEP, LOW);
}

void iniciaEstabilizacao() {
  estabilizando = true;
  estabilizaAteMs = millis() + TEMPO_ESTABILIZA_MS;
  paraMotor();
}

// --- PARAGEM CONTROLADA EM PISO PEDIDO (10 s) ---
const unsigned long TEMPO_PARAGEM_PISO_MS = 10000UL;
bool paragemPisoAtiva = false;
unsigned long paragemPisoAteMs = 0;

void iniciaParagemPisoPedido() {
  paragemPisoAtiva = true;
  paragemPisoAteMs = millis() + TEMPO_PARAGEM_PISO_MS;

  estabilizando = false;
  acaoElevador = 0;
  andarDestino = andar;
  paraMotor();
}

// --- DEBOUNCE DOS BOTÕES DE PEDIDO (50 ms) ---
// ordem:
// 0..3 exteriores
// 4..7 interiores
const unsigned long DEBOUNCE_MS = 50;
const int NUM_BOTOES_PEDIDO = 8;

const int botoesPedido[NUM_BOTOES_PEDIDO] = {
  pinBotaoExtA, pinBotaoExtB, pinBotaoExtC, pinBotaoExtD,
  pinBotaoIntA, pinBotaoIntB, pinBotaoIntC, pinBotaoIntD
};

// todos mapeiam para os mesmos 4 pisos
const int mapaPedidoBotao[NUM_BOTOES_PEDIDO] = {
  0, 1, 2, 3,
  0, 1, 2, 3
};

bool botaoRawAnt[NUM_BOTOES_PEDIDO];
bool botaoEstavel[NUM_BOTOES_PEDIDO];
unsigned long botaoUltMudancaMs[NUM_BOTOES_PEDIDO];

void debounceBotoesERegistaPedidos() {
  unsigned long agora = millis();

  for (int i = 0; i < NUM_BOTOES_PEDIDO; i++) {
    bool raw = digitalRead(botoesPedido[i]);

    if (raw != botaoRawAnt[i]) {
      botaoRawAnt[i] = raw;
      botaoUltMudancaMs[i] = agora;
    }

    if ((agora - botaoUltMudancaMs[i]) >= DEBOUNCE_MS) {
      if (raw != botaoEstavel[i]) {
        bool antigo = botaoEstavel[i];
        botaoEstavel[i] = raw;

        // flanco HIGH -> LOW = botão premido
        if (antigo == HIGH && botaoEstavel[i] == LOW) {
          pedidos[mapaPedidoBotao[i]] = true;
        }
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

// --- DEBOUNCE DA PORTA ---
bool portaRawAnt = LOW;
bool portaEstavel = LOW;
unsigned long portaUltMudancaMs = 0;

void debouncePortaPermissiva() {
  unsigned long agora = millis();
  bool raw = digitalRead(pinPortaPermissiva);

  if (raw != portaRawAnt) {
    portaRawAnt = raw;
    portaUltMudancaMs = agora;
  }

  if ((agora - portaUltMudancaMs) >= DEBOUNCE_MS) {
    portaEstavel = raw;
  }
}

bool portaPermissivaValida() {
  return (portaEstavel == HIGH);
}

// ======= STEP SERVICE (A4988) =======
void stepperService() {
  if (stepRate <= 0 || motorDir == 0) return;

  digitalWrite(PIN_DIR, (motorDir > 0) ? HIGH : LOW);
  if (PIN_EN != -1) digitalWrite(PIN_EN, LOW);

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

enum FaseRearme {
  REARME_NONE,
  REARME_ATE_PISO_VALIDO,
  REARME_PAUSA_2S,
  REARME_IR_PISO1,
  REARME_ESPERA_10S
};
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

  for (int i = 0; i < 4; i++) {
    digitalWrite(ledsExt[i], LOW);
    digitalWrite(ledsInt[i], LOW);
  }
}

void entraErroMovendo() {
  estadoSistema = ERRO_MOVENDO;
  iniciaAlarmeContinuo();
  limpaPedidosEDesligaLEDs();
  estabilizando = false;
  paragemPisoAtiva = false;
  paraMotor();
}

void fixaErroParado() {
  estadoSistema = ERRO_PARADO;
  paragemPisoAtiva = false;
  paraMotor();
  acaoElevador = 0;
  erroPortaAteMs = millis() + ERRO_PORTA_MS;
}

void iniciaRearme() {
  estadoSistema = REARME_ATIVO;
  limpaPedidosEDesligaLEDs();
  iniciaAlarmeContinuo();
  estabilizando = false;
  paragemPisoAtiva = false;

  if (acaoElevador == 1 || acaoElevador == 2) {
    faseRearme = REARME_ATE_PISO_VALIDO;
  } else {
    faseRearme = REARME_PAUSA_2S;
    faseAteMs = millis() + 2000UL;
    paraMotor();
    acaoElevador = 0;
  }
}

// --- helpers ---
bool haPedidosPendentes() {
  for (int i = 0; i < 4; i++) if (pedidos[i]) return true;
  return false;
}

int proximoAcima(int andarAtual) {
  for (int i = andarAtual; i < 4; i++) {
    if (pedidos[i]) return i + 1;
  }
  return -1;
}

int proximoAbaixo(int andarAtual) {
  for (int i = andarAtual - 2; i >= 0; i--) {
    if (pedidos[i]) return i + 1;
  }
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
    if (dest == -1) {
      dest = proximoAbaixo(andar);
      if (dest != -1) sentidoAtual = -1;
    }
  } else if (sentidoAtual == -1) {
    dest = proximoAbaixo(andar);
    if (dest == -1) {
      dest = proximoAcima(andar);
      if (dest != -1) sentidoAtual = 1;
    }
  } else {
    dest = proximoAcima(andar);
    if (dest == -1) dest = proximoAbaixo(andar);
    if (dest != -1) sentidoAtual = (dest > andar) ? 1 : -1;
  }

  if (dest == -1) dest = andar;
  andarDestino = dest;

  if (andarDestino == andar) {
    acaoElevador = 0;
  } else if (andarDestino > andar) {
    acaoElevador = 1;
    sentidoAtual = 1;
  } else {
    acaoElevador = 2;
    sentidoAtual = -1;
  }
}

void atendePedidoNoPisoAtualSeExistir() {
  if (estadoSistema != NORMAL) return;
  if (andar < 1 || andar > 4) return;
  if (estabilizando || paragemPisoAtiva) return;
  if (acaoElevador != 0) return;

  int idxAtual = andar - 1;

  if (pedidos[idxAtual]) {
    beepCurto();
    pedidos[idxAtual] = false;
    iniciaParagemPisoPedido();
  }
}

void processaChegadaPiso(int piso, int idxPedido) {
  andar = piso;

  if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_ATE_PISO_VALIDO) {
      paraMotor();
      acaoElevador = 0;
      faseRearme = REARME_PAUSA_2S;
      faseAteMs = millis() + 2000UL;
      return;
    }

    if (faseRearme == REARME_IR_PISO1) {
      if (piso == 1) {
        paraMotor();
        acaoElevador = 0;
        faseRearme = REARME_ESPERA_10S;
        faseAteMs = millis() + 10000UL;
      }
      return;
    }

    return;
  }

  if (estadoSistema == ERRO_MOVENDO) {
    fixaErroParado();
    return;
  }

  if (estadoSistema == ERRO_PARADO) return;

  bool tinhaPedido = pedidos[idxPedido];

  if (tinhaPedido) beepCurto();
  pedidos[idxPedido] = false;

  if (tinhaPedido) {
    iniciaParagemPisoPedido();
  } else if (andarDestino == piso) {
    iniciaEstabilizacao();
  }
}

// ===================== OLED x4 via TCA9548A =====================
unsigned long lastOledMs = 0;
bool pedidosAntOLED[4] = { false, false, false, false };
int pedidoRecenteOLED = -1;

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

void renderFila(Adafruit_SSD1306 &d, int xStart, int yStart) {
  int x = xStart;
  int y = yStart;
  int cont = 0;

  if (andar >= 1 && andar <= 4) {
    if (sentidoAtual == 1) {
      for (int p = andar; p <= 4; p++) {
        if (pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
      for (int p = andar - 1; p >= 1; p--) {
        if (pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
    } else if (sentidoAtual == -1) {
      for (int p = andar; p >= 1; p--) {
        if (pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
      for (int p = andar + 1; p <= 4; p++) {
        if (pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
    } else {
      for (int p = 1; p <= 4; p++) {
        if (pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
    }
  } else {
    for (int p = 1; p <= 4; p++) {
      if (pedidos[p - 1]) {
        d.setCursor(x, y); d.print(p); d.print(' ');
        x += 12; cont++;
        if (x > 120) { x = 0; y += 10; }
      }
    }
  }

  if (cont == 0) {
    d.setCursor(xStart, yStart);
    d.print("-");
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
  renderFila(d, 30, 46);

  d.setCursor(0, 58);
  if (estadoSistema == NORMAL) {
    if (paragemPisoAtiva) d.print("PARAGEM");
    else if (!portaPermissivaValida()) d.print("PORTA");
    else if (estabilizando) d.print("ESTAB");
    else d.print("NORMAL");
  } else if (estadoSistema == ERRO_MOVENDO) {
    d.print("ERRO_MOV");
  } else if (estadoSistema == ERRO_PARADO) {
    d.print("ERRO_PAR");
  } else {
    d.print("REARME");
  }

  d.display();
}

void oledRenderAll4() {
  tcaSelect(0); renderOneOLED(oled1, 1);
  tcaSelect(1); renderOneOLED(oled2, 2);
  tcaSelect(2); renderOneOLED(oled3, 3);
  tcaSelect(3); renderOneOLED(oled4, 4);
}

// ===================== SETUP =====================
void setup() {
  // Botões exteriores
  pinMode(pinBotaoExtA, INPUT_PULLUP);
  pinMode(pinBotaoExtB, INPUT_PULLUP);
  pinMode(pinBotaoExtC, INPUT_PULLUP);
  pinMode(pinBotaoExtD, INPUT_PULLUP);

  // Botões interiores
  pinMode(pinBotaoIntA, INPUT_PULLUP);
  pinMode(pinBotaoIntB, INPUT_PULLUP);
  pinMode(pinBotaoIntC, INPUT_PULLUP);
  pinMode(pinBotaoIntD, INPUT_PULLUP);

  pinMode(pinReset, INPUT_PULLUP);

  // LEDs exteriores
  pinMode(pinLedExtA, OUTPUT);
  pinMode(pinLedExtB, OUTPUT);
  pinMode(pinLedExtC, OUTPUT);
  pinMode(pinLedExtD, OUTPUT);

  // LEDs interiores
  pinMode(pinLedIntA, OUTPUT);
  pinMode(pinLedIntB, OUTPUT);
  pinMode(pinLedIntC, OUTPUT);
  pinMode(pinLedIntD, OUTPUT);

  pinMode(pinSensorA, INPUT_PULLUP);
  pinMode(pinSensorB, INPUT_PULLUP);
  pinMode(pinSensorC, INPUT_PULLUP);
  pinMode(pinSensorD, INPUT_PULLUP);

  pinMode(pinBuzzer, OUTPUT);

  // Porta: usar switch a dar 5V ou GND
  pinMode(pinPortaPermissiva, INPUT);

  // A4988
  pinMode(PIN_STEP, OUTPUT);
  pinMode(PIN_DIR, OUTPUT);
  if (PIN_EN != -1) pinMode(PIN_EN, OUTPUT);

  digitalWrite(PIN_STEP, LOW);
  digitalWrite(PIN_DIR, LOW);
  if (PIN_EN != -1) digitalWrite(PIN_EN, HIGH);

  // apagar LEDs no arranque
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledsExt[i], LOW);
    digitalWrite(ledsInt[i], LOW);
  }

  // I2C + OLEDs via TCA
  Wire.begin();

  tcaSelect(0); oled1.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  tcaSelect(1); oled2.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  tcaSelect(2); oled3.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  tcaSelect(3); oled4.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);

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

    paraMotor();
  }

  andarDestino = andar;
  acaoElevador = 0;
  sentidoAtual = 0;

  estadoSensorAAnt = estadoSensorA;
  estadoSensorBAnt = estadoSensorB;
  estadoSensorCAnt = estadoSensorC;
  estadoSensorDAnt = estadoSensorD;

  unsigned long agora = millis();

  for (int i = 0; i < NUM_BOTOES_PEDIDO; i++) {
    bool raw = digitalRead(botoesPedido[i]);
    botaoRawAnt[i] = raw;
    botaoEstavel[i] = raw;
    botaoUltMudancaMs[i] = agora;
  }

  resetRawAnt = digitalRead(pinReset);
  resetEstavel = resetRawAnt;
  resetUltMudancaMs = agora;

  portaRawAnt = digitalRead(pinPortaPermissiva);
  portaEstavel = portaRawAnt;
  portaUltMudancaMs = agora;

  for (int i = 0; i < 4; i++) {
    pedidosAntOLED[i] = pedidos[i];
  }

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

  debouncePortaPermissiva();

  if (resetFoiPremido()) iniciaRearme();

  estadoSensorA = !digitalRead(pinSensorA);
  estadoSensorB = !digitalRead(pinSensorB);
  estadoSensorC = !digitalRead(pinSensorC);
  estadoSensorD = !digitalRead(pinSensorD);

  if (estadoSistema == NORMAL) {
    debounceBotoesERegistaPedidos();
  }

  if (estadoSensorA && !estadoSensorAAnt) processaChegadaPiso(1, 0);
  if (estadoSensorB && !estadoSensorBAnt) processaChegadaPiso(2, 1);
  if (estadoSensorC && !estadoSensorCAnt) processaChegadaPiso(3, 2);
  if (estadoSensorD && !estadoSensorDAnt) processaChegadaPiso(4, 3);

  // atende pedido no piso atual mesmo sem novo flanco
  atendePedidoNoPisoAtualSeExistir();

  if (estabilizando && millis() >= estabilizaAteMs) {
    estabilizando = false;
  }

  // fim da paragem do piso pedido:
  // só sai quando os 10 s passaram e a porta está fechada
  if (paragemPisoAtiva) {
    if ((millis() >= paragemPisoAteMs) && portaPermissivaValida()) {
      paragemPisoAtiva = false;
      selecionaDestinoEAcao();
    }
  }

  if (estadoSistema == NORMAL) {
    if (!estabilizando && !paragemPisoAtiva && stepRate == 0 && portaPermissivaValida()) {
      selecionaDestinoEAcao();
    }

    bool movimentoCmd =
      (!estabilizando &&
       !paragemPisoAtiva &&
       portaPermissivaValida() &&
       (acaoElevador == 1 || acaoElevador == 2));

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
      paragemPisoAtiva = false;

      for (int i = 0; i < 4; i++) pedidos[i] = false;
      acaoElevador = 0;

      paraMotor();
      andarDestino = andar;
      sentidoAtual = 0;
    }
  }

  // comando final do motor
  if (estadoSistema == ERRO_PARADO) {
    paraMotor();

  } else if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_PAUSA_2S || faseRearme == REARME_ESPERA_10S) {
      paraMotor();
    } else {
      if (acaoElevador == 0) {
        paraMotor();
      } else if (acaoElevador == 1) {
        stepRate = velocidadeSobe;
        motorDir = +1;
      } else if (acaoElevador == 2) {
        stepRate = velocidadeDesce;
        motorDir = -1;
      }
    }

  } else if (estabilizando || paragemPisoAtiva || !portaPermissivaValida()) {
    paraMotor();

  } else {
    if (acaoElevador == 0) {
      paraMotor();
    } else if (acaoElevador == 1) {
      stepRate = velocidadeSobe;
      motorDir = +1;
    } else if (acaoElevador == 2) {
      stepRate = velocidadeDesce;
      motorDir = -1;
    }
  }

  // LEDs exteriores + interiores
  if (estadoSistema == NORMAL) {
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledsExt[i], pedidos[i] ? HIGH : LOW);
      digitalWrite(ledsInt[i], pedidos[i] ? HIGH : LOW);
    }
  } else {
    for (int i = 0; i < 4; i++) {
      digitalWrite(ledsExt[i], LOW);
      digitalWrite(ledsInt[i], LOW);
    }
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
