#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <stdio.h>

// ================= PINAGEM (IGUAL AO TEU PRINT) =================

// --- A4988 (Stepper) ---
#define pinSTEP 9
#define pinDIR  10
#define velocidadeSobe   600
#define velocidadeDesce  350

// --- Botões de chamada ---
#define pinBotaoA 2
#define pinBotaoB 3
#define pinBotaoC 4
#define pinBotaoD 5

// --- RESET ---
#define pinReset 7   // botão mais acima (INPUT_PULLUP)

// --- Sensores de piso ---
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

// ================================================================

hd44780_I2Cexp lcd;

String ponteiro[2] = { "v", " " };
int ledAndar[4] = { pinLedA, pinLedB, pinLedC, pinLedD };

// ---------------- 7 SEGMENTOS (4 displays) - MULTIPLEX ----------------
// Ajusta estes pinos se ligaste diferente:
const int segmentPins[7] = { 22, 23, 24, 25, 26, 27, 28 }; // A,B,C,D,E,F,G
const int digitPins[4]   = { 30, 31, 32, 33 };             // COM dos 4 displays (piso1..piso4)

// Wokwi default: common anode
const bool COMMON_ANODE = true;

// refrescamento: 2000us por dígito -> ~125Hz ciclo completo
const unsigned long MUX_INTERVAL_US = 2000;
unsigned long lastMuxUs = 0;
int muxIndex = 0;

// Máscaras de segmentos (A..G) para 0-9 (1 = segmento ON)
const byte DIGITS[10] = {
  //  gfedcba (vamos mapear A..G com bits 0..6)
  0b00111111, // 0: a b c d e f
  0b00000110, // 1: b c
  0b01011011, // 2: a b d e g
  0b01001111, // 3: a b c d g
  0b01100110, // 4: b c f g
  0b01101101, // 5: a c d f g
  0b01111101, // 6: a c d e f g
  0b00000111, // 7: a b c
  0b01111111, // 8: a b c d e f g
  0b01101111  // 9: a b c d f g
};

void allDigitsOff() {
  for (int i = 0; i < 4; i++) {
    // common anode: OFF = LOW
    digitalWrite(digitPins[i], COMMON_ANODE ? LOW : HIGH);
  }
}

void setSegmentsMask(byte mask) {
  // bits 0..6 correspondem a A..G
  for (int s = 0; s < 7; s++) {
    bool on = (mask >> s) & 0x01;
    if (COMMON_ANODE) {
      // ON = LOW, OFF = HIGH
      digitalWrite(segmentPins[s], on ? LOW : HIGH);
    } else {
      // common cathode: ON = HIGH, OFF = LOW
      digitalWrite(segmentPins[s], on ? HIGH : LOW);
    }
  }
}

void displayService(int valor) {
  // valor esperado: 0..9; se -1 -> apagado
  unsigned long now = micros();
  if (now - lastMuxUs < MUX_INTERVAL_US) return;
  lastMuxUs = now;

  // 1) desligar todos os dígitos
  allDigitsOff();

  // 2) definir segmentos para o valor
  if (valor < 0 || valor > 9) {
    setSegmentsMask(0); // tudo apagado
  } else {
    setSegmentsMask(DIGITS[valor]);
  }

  // 3) ligar o dígito atual
  if (COMMON_ANODE) digitalWrite(digitPins[muxIndex], HIGH); // ON
  else             digitalWrite(digitPins[muxIndex], LOW);   // ON

  muxIndex++;
  if (muxIndex >= 4) muxIndex = 0;
}

// ---------------- LÓGICA ELEVADOR ----------------

// Pedidos
bool pedidos[4] = { false, false, false, false };

bool estadoSensorA, estadoSensorB, estadoSensorC, estadoSensorD;
bool estadoSensorAAnt, estadoSensorBAnt, estadoSensorCAnt, estadoSensorDAnt;

int acaoElevador = 0;  // 0=parado, 1=subir, 2=descer
int andar = -1;
int andarDestino = -1;

int sentidoAtual = 0; // 1=subir, -1=descer, 0=indefinido

// --- Stepper runtime ---
volatile int stepRate = 0;
unsigned long lastStepMicros = 0;

// --- Buzzer curto (não-bloqueante) ---
bool beepAtivo = false;
unsigned long beepAteMs = 0;

// cadências
unsigned long lastLogicMs = 0;
unsigned long lastLcdMs   = 0;

// --- ESTABILIZAÇÃO EM PISO (300ms) ---
const unsigned long TEMPO_ESTABILIZA_MS = 300;
bool estabilizando = false;
unsigned long estabilizaAteMs = 0;

void iniciaEstabilizacao() {
  estabilizando = true;
  estabilizaAteMs = millis() + TEMPO_ESTABILIZA_MS;
  stepRate = 0;
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
    bool raw = digitalRead(botoes[i]); // HIGH solto, LOW premido

    if (raw != botaoRawAnt[i]) {
      botaoRawAnt[i] = raw;
      botaoUltMudancaMs[i] = agora;
    }

    if ((agora - botaoUltMudancaMs[i]) >= DEBOUNCE_MS) {
      if (raw != botaoEstavel[i]) {
        bool antigo = botaoEstavel[i];
        botaoEstavel[i] = raw;

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

void stepperService() {
  if (stepRate <= 0) return;

  unsigned long interval = 1000000UL / (unsigned long)stepRate;
  unsigned long now = micros();

  if (now - lastStepMicros >= interval) {
    lastStepMicros = now;
    digitalWrite(pinSTEP, HIGH);
    delayMicroseconds(3);
    digitalWrite(pinSTEP, LOW);
  }
}

void beepService() {
  if (beepAtivo && millis() >= beepAteMs) {
    noTone(pinBuzzer);
    beepAtivo = false;
  }
}

void atualizaLcdPonteiro() {
  lcd.setCursor(1, 0);  lcd.print(ponteiro[digitalRead(pinSensorA)]);
  lcd.setCursor(5, 0);  lcd.print(ponteiro[digitalRead(pinSensorB)]);
  lcd.setCursor(9, 0);  lcd.print(ponteiro[digitalRead(pinSensorC)]);
  lcd.setCursor(13, 0); lcd.print(ponteiro[digitalRead(pinSensorD)]);
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
unsigned long lastErroLcdMs = 0;

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

void mostraErroMovendoNoLcd() {
  lcd.setCursor(0, 2); lcd.print("ERRO: TIMEOUT       ");
  lcd.setCursor(0, 3); lcd.print("Atingir piso...     ");
}

void mostraErroParadoNoLcd() {
  lcd.setCursor(0, 2); lcd.print("ERRO_TIMEOUT        ");
  lcd.setCursor(0, 3); lcd.print("Aguardar RESET      ");
}

void mostraRearmeNoLcd(const char* linha3) {
  lcd.setCursor(0, 2); lcd.print("REARME_ATIVO        ");
  lcd.setCursor(0, 3); lcd.print(linha3);
}

void entraErroMovendo() {
  estadoSistema = ERRO_MOVENDO;
  iniciaAlarmeContinuo();
  limpaPedidosEDesligaLEDs();
  estabilizando = false;
  mostraErroMovendoNoLcd();
}

void fixaErroParado() {
  estadoSistema = ERRO_PARADO;
  stepRate = 0;
  acaoElevador = 0;
  erroPortaAteMs = millis() + ERRO_PORTA_MS;
  lastErroLcdMs = 0;
  mostraErroParadoNoLcd();
}

void iniciaRearme() {
  estadoSistema = REARME_ATIVO;
  limpaPedidosEDesligaLEDs();
  iniciaAlarmeContinuo();
  estabilizando = false;

  if (acaoElevador == 1 || acaoElevador == 2) {
    faseRearme = REARME_ATE_PISO_VALIDO;
    mostraRearmeNoLcd("Atingir piso...     ");
  } else {
    faseRearme = REARME_PAUSA_2S;
    faseAteMs = millis() + 2000UL;
    stepRate = 0;
    acaoElevador = 0;
    mostraRearmeNoLcd("Parado 2s...        ");
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
      stepRate = 0; acaoElevador = 0;
      faseRearme = REARME_PAUSA_2S;
      faseAteMs = millis() + 2000UL;
      mostraRearmeNoLcd("Parado 2s...        ");
      return;
    }
    if (faseRearme == REARME_IR_PISO1) {
      if (piso == 1) {
        stepRate = 0; acaoElevador = 0;
        faseRearme = REARME_ESPERA_10S;
        faseAteMs = millis() + 10000UL;
        mostraRearmeNoLcd("No piso 1 (10s)     ");
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

  pinMode(pinSTEP, OUTPUT);
  pinMode(pinDIR, OUTPUT);
  digitalWrite(pinSTEP, LOW);

  // --------- init 7seg pins ----------
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
    // OFF por defeito
    digitalWrite(segmentPins[i], COMMON_ANODE ? HIGH : LOW);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], COMMON_ANODE ? LOW : HIGH); // dígitos OFF
  }
  allDigitsOff();
  setSegmentsMask(0);

  lcd.begin(20, 4);
  lcd.backlight();

  lcd.setCursor(0, 1);
  lcd.print("(1) (2) (3) (4)");

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

  if (andar == -1) {
    digitalWrite(pinDIR, LOW);
    stepRate = velocidadeDesce;
    while (andar == -1) {
      stepperService();
      beepService();
      displayService(0); // manter refresh

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
  }

  resetRawAnt = digitalRead(pinReset);
  resetEstavel = resetRawAnt;
  resetUltMudancaMs = agora;
}

void loop() {
  // serviços permanentes
  stepperService();
  beepService();

  // --------- 7 segmentos: mostrar andar atual em TODOS os pisos ----------
  int valorMostrado = (andar >= 1 && andar <= 4) ? andar : 0;
  displayService(valorMostrado);

  // lógica a cada 10ms
  if (millis() - lastLogicMs < 10) return;
  lastLogicMs = millis();

  // RESET tem prioridade
  if (resetFoiPremido()) iniciaRearme();

  // ler sensores (ativos a LOW)
  estadoSensorA = !digitalRead(pinSensorA);
  estadoSensorB = !digitalRead(pinSensorB);
  estadoSensorC = !digitalRead(pinSensorC);
  estadoSensorD = !digitalRead(pinSensorD);

  // botões só em NORMAL
  if (estadoSistema == NORMAL) debounceBotoesERegistaPedidos();

  // chegadas a piso (edge)
  if (estadoSensorA && !estadoSensorAAnt) processaChegadaPiso(1, 0);
  if (estadoSensorB && !estadoSensorBAnt) processaChegadaPiso(2, 1);
  if (estadoSensorC && !estadoSensorCAnt) processaChegadaPiso(3, 2);
  if (estadoSensorD && !estadoSensorDAnt) processaChegadaPiso(4, 3);

  // fim estabilização
  if (estabilizando && millis() >= estabilizaAteMs) estabilizando = false;

  // NORMAL: decisão + timeout
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

  // REARME: sequência
  if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_PAUSA_2S && millis() >= faseAteMs) {
      if (andar == 1) {
        faseRearme = REARME_ESPERA_10S;
        faseAteMs = millis() + 10000UL;
        mostraRearmeNoLcd("No piso 1 (10s)     ");
      } else {
        faseRearme = REARME_IR_PISO1;
        rearmeMoveStartMs = millis();
        andarDestino = 1;
        acaoElevador = 2;
        sentidoAtual = -1;
        mostraRearmeNoLcd("A descer p/ piso 1  ");
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
      andarDestino = andar;
      sentidoAtual = 0;

      lcd.setCursor(0, 2); lcd.print("                    ");
      lcd.setCursor(0, 3); lcd.print("                    ");
    }
  }

  // motor
  if (estadoSistema == ERRO_PARADO) {
    stepRate = 0;
  } else if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_PAUSA_2S || faseRearme == REARME_ESPERA_10S) {
      stepRate = 0;
    } else {
      if (acaoElevador == 0) stepRate = 0;
      if (acaoElevador == 1) { digitalWrite(pinDIR, HIGH); stepRate = velocidadeSobe; }
      if (acaoElevador == 2) { digitalWrite(pinDIR, LOW);  stepRate = velocidadeDesce; }
    }
  } else if (estabilizando) {
    stepRate = 0;
  } else {
    if (acaoElevador == 0) stepRate = 0;
    if (acaoElevador == 1) { digitalWrite(pinDIR, HIGH); stepRate = velocidadeSobe; }
    if (acaoElevador == 2) { digitalWrite(pinDIR, LOW);  stepRate = velocidadeDesce; }
  }

  // LEDs pedidos
  if (estadoSistema == NORMAL) {
    for (int i = 0; i < 4; i++) digitalWrite(ledAndar[i], pedidos[i] ? HIGH : LOW);
  } else {
    for (int i = 0; i < 4; i++) digitalWrite(ledAndar[i], LOW);
  }

  // LCD ponteiro
  if (millis() - lastLcdMs >= 100) {
    lastLcdMs = millis();
    atualizaLcdPonteiro();
  }

  // ERRO_PARADO: countdown 2 min
  if (estadoSistema == ERRO_PARADO) {
    if (millis() - lastErroLcdMs >= 1000) {
      lastErroLcdMs = millis();
      long restante = (long)(erroPortaAteMs - millis());
      if (restante < 0) restante = 0;
      long sec = restante / 1000;

      lcd.setCursor(0, 3);
      char buf[21];
      snprintf(buf, sizeof(buf), "Porta logica: %3lds ", sec);
      lcd.print(buf);
    }
  }

  estadoSensorAAnt = estadoSensorA;
  estadoSensorBAnt = estadoSensorB;
  estadoSensorCAnt = estadoSensorC;
  estadoSensorDAnt = estadoSensorD;
}
