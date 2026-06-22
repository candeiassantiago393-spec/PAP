/*
  ELEVADOR 4 PISOS — L298N + PORTA + PEDIDOS INTERIORES + 4 OLED SH1106
  ----------------------------------------------------------------------
  OTIMIZAÇÃO DOS DISPLAYS:
  - só atualiza quando há mudança
  - atualiza 1 display de cada vez
  - reduz carga sobre o motor

  Displays:
  - 4x OLED 1.3" I2C SH1106 via TCA9548A
  - OLED_ADDR típico: 0x3C
  - TCA_ADDR típico: 0x70

  Arduino Mega:
  - SDA = 20
  - SCL = 21

  PORTA:
  - sensor Hall em D22
  - LOW = magnetizado = porta fechada = permissiva válida
  - HIGH = porta aberta = movimento bloqueado
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// ================= OLED =================
#define OLED_W    128
#define OLED_H    64
#define OLED_ADDR 0x3C
#define TCA_ADDR  0x70

Adafruit_SH1106G oled1(OLED_W, OLED_H, &Wire, -1);
Adafruit_SH1106G oled2(OLED_W, OLED_H, &Wire, -1);
Adafruit_SH1106G oled3(OLED_W, OLED_H, &Wire, -1);
Adafruit_SH1106G oled4(OLED_W, OLED_H, &Wire, -1);

// ================= PINAGEM =================

// --- L298N ---
#define pinMotorIN1 9
#define pinMotorIN2 33
#define pinMotorIN3 31
#define pinMotorIN4 32

// velocidades
#define velocidadeSobe  80
#define velocidadeDesce 80

// --- Botões exteriores ---
#define pinBotaoExt1 2
#define pinBotaoExt2 3
#define pinBotaoExt3 4
#define pinBotaoExt4 5

// --- Buzzer ---
#define pinBuzzer 6

// --- Reset ---
#define pinReset 7

// --- Sensores Hall dos pisos (ativos a LOW) ---
#define pinSensor1 8
#define pinSensor2 11
#define pinSensor3 12
#define pinSensor4 13

// --- Sensor Hall da porta ---
#define pinPortaPermissiva 22

// --- Botões interiores ---
#define pinBotaoInt1 23
#define pinBotaoInt2 24
#define pinBotaoInt3 25
#define pinBotaoInt4 26

// --- LEDs interiores ---
#define pinLedInt1 27
#define pinLedInt2 28
#define pinLedInt3 29
#define pinLedInt4 30

// --- LEDs exteriores ---
#define pinLedExt1 A0
#define pinLedExt2 A1
#define pinLedExt3 A2
#define pinLedExt4 A3

// ================= ARRAYS =================
const int ledsExt[4] = { pinLedExt1, pinLedExt2, pinLedExt3, pinLedExt4 };
const int ledsInt[4] = { pinLedInt1, pinLedInt2, pinLedInt3, pinLedInt4 };

const int sensoresHall[4] = { pinSensor1, pinSensor2, pinSensor3, pinSensor4 };

// 8 botões -> 4 pisos
const int NUM_BOTOES_PEDIDO = 8;
const int botoesPedido[NUM_BOTOES_PEDIDO] = {
  pinBotaoExt1, pinBotaoExt2, pinBotaoExt3, pinBotaoExt4,
  pinBotaoInt1, pinBotaoInt2, pinBotaoInt3, pinBotaoInt4
};

const int mapaPedidoBotao[NUM_BOTOES_PEDIDO] = {
  0, 1, 2, 3,
  0, 1, 2, 3
};

// ================= LÓGICA =================
bool pedidos[4] = { false, false, false, false };

bool estadoSensor[4];
bool estadoSensorAnt[4];

int acaoElevador = 0;   // 0=parado, 1=subir, 2=descer
int andar = -1;
int andarDestino = -1;
int sentidoAtual = 0;   // 1=subir, -1=descer, 0=indefinido
int pedidoRecenteOLED = -1;

// ================= MOTOR L298N =================
volatile int stepRate = 0;
unsigned long lastStepMicros = 0;
int motorDir = 0;
int faseMotor = 0;

// ================= BUZZER =================
bool beepAtivo = false;
unsigned long beepAteMs = 0;
bool alarmeAtivo = false;

// ================= TEMPOS =================
unsigned long lastLogicMs = 0;

const unsigned long TEMPO_ESTABILIZA_MS = 300;
bool estabilizando = false;
unsigned long estabilizaAteMs = 0;

const unsigned long TEMPO_PARAGEM_PISO_MS = 10000UL;
bool paragemPisoAtiva = false;
unsigned long paragemPisoAteMs = 0;

const unsigned long DEBOUNCE_MS = 50;
const unsigned long TIMEOUT_MOV_MS = 15000UL;

// ================= ESTADOS =================
enum EstadoSistema { NORMAL, ERRO_MOVENDO, ERRO_PARADO, REARME_ATIVO };
EstadoSistema estadoSistema = NORMAL;

bool movimentoAtivo = false;
unsigned long inicioMovMs = 0;

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

// ================= DEBOUNCE BOTÕES =================
bool botaoRawAnt[NUM_BOTOES_PEDIDO];
bool botaoEstavel[NUM_BOTOES_PEDIDO];
unsigned long botaoUltMudancaMs[NUM_BOTOES_PEDIDO];

// ================= DEBOUNCE RESET =================
bool resetRawAnt = HIGH;
bool resetEstavel = HIGH;
unsigned long resetUltMudancaMs = 0;

// ================= DEBOUNCE PORTA =================
bool portaRawAnt = HIGH;
bool portaEstavel = HIGH;
unsigned long portaUltMudancaMs = 0;

// ================= OLED OTIMIZADO =================
const unsigned long OLED_INTERVALO_MS = 300;
unsigned long lastOledMs = 0;
byte oledCanalAtual = 0;
bool oledCicloAtivo = false;

enum OledModoEstado {
  OLED_EST_NORMAL = 0,
  OLED_EST_PORTA,
  OLED_EST_PARAGEM,
  OLED_EST_ESTAB,
  OLED_EST_ERRO_MOV,
  OLED_EST_ERRO_PAR,
  OLED_EST_REARME
};

typedef struct {
  int andar;
  int andarDestino;
  int sentidoAtual;
  int acaoElevador;
  int pedidoRecente;
  uint8_t modoEstado;
  bool pedidos[4];
} OledSnapshot;

OledSnapshot oledUltimoCompleto;
OledSnapshot oledAlvo;

// ================= PROTÓTIPOS =================
void aplicaFaseMotor(int fase);
void desligaBobinasMotor();
void paraMotor();
void motorService();

void iniciaAlarmeContinuo();
void desligaAlarmeContinuo();
void beepCurto();
void buzzerService();

void iniciaEstabilizacao();
void iniciaParagemPisoPedido();

bool resetFoiPremido();
void debouncePortaPermissiva();
bool portaPermissivaValida();
void debounceBotoesERegistaPedidos();

void limpaPedidosEDesligaLEDs();
bool haPedidosPendentes();
int proximoAcima(int andarAtual);
int proximoAbaixo(int andarAtual);
void selecionaDestinoEAcao();
void atendePedidoNoPisoAtualSeExistir();

void entraErroMovendo();
void fixaErroParado();
void iniciaRearme();
void processaChegadaPiso(int piso, int idxPedido);

void tcaSelect(uint8_t channel);
char dirSetaOLED(int acao);
uint8_t calculaModoEstadoOLED();
OledSnapshot criaSnapshotAtual();
bool snapshotsIguais(const OledSnapshot &a, const OledSnapshot &b);
void renderFila(Adafruit_SH1106G &d, int xStart, int yStart, const OledSnapshot &s);
void renderEstadoOLED(Adafruit_SH1106G &d, uint8_t modoEstado);
void renderOneOLED(Adafruit_SH1106G &d, int idx, const OledSnapshot &s);
void oledAtualizaUmCanal();
void oledService();

// ===================================================

void aplicaFaseMotor(int fase) {
  switch (fase & 0x03) {
    case 0:
      digitalWrite(pinMotorIN1, HIGH);
      digitalWrite(pinMotorIN2, LOW);
      digitalWrite(pinMotorIN3, HIGH);
      digitalWrite(pinMotorIN4, LOW);
      break;

    case 1:
      digitalWrite(pinMotorIN1, LOW);
      digitalWrite(pinMotorIN2, HIGH);
      digitalWrite(pinMotorIN3, HIGH);
      digitalWrite(pinMotorIN4, LOW);
      break;

    case 2:
      digitalWrite(pinMotorIN1, LOW);
      digitalWrite(pinMotorIN2, HIGH);
      digitalWrite(pinMotorIN3, LOW);
      digitalWrite(pinMotorIN4, HIGH);
      break;

    case 3:
      digitalWrite(pinMotorIN1, HIGH);
      digitalWrite(pinMotorIN2, LOW);
      digitalWrite(pinMotorIN3, LOW);
      digitalWrite(pinMotorIN4, HIGH);
      break;
  }
}

void desligaBobinasMotor() {
  digitalWrite(pinMotorIN1, LOW);
  digitalWrite(pinMotorIN2, LOW);
  digitalWrite(pinMotorIN3, LOW);
  digitalWrite(pinMotorIN4, LOW);
}

void paraMotor() {
  stepRate = 0;
  motorDir = 0;
  desligaBobinasMotor();
}

void motorService() {
  if (stepRate <= 0 || motorDir == 0) return;

  unsigned long now = micros();
  unsigned long intervalo = 1000000UL / (unsigned long)stepRate;

  if (now - lastStepMicros >= intervalo) {
    lastStepMicros = now;

    if (motorDir > 0) {
      faseMotor = (faseMotor + 1) & 0x03;
    } else {
      faseMotor = (faseMotor + 3) & 0x03;
    }

    aplicaFaseMotor(faseMotor);
  }
}

void iniciaAlarmeContinuo() {
  if (alarmeAtivo) return;
  beepAtivo = false;
  tone(pinBuzzer, 900);
  alarmeAtivo = true;
}

void desligaAlarmeContinuo() {
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

void buzzerService() {
  if (beepAtivo && millis() >= beepAteMs) {
    noTone(pinBuzzer);
    beepAtivo = false;
  }
}

void iniciaEstabilizacao() {
  estabilizando = true;
  estabilizaAteMs = millis() + TEMPO_ESTABILIZA_MS;
  paraMotor();
}

void iniciaParagemPisoPedido() {
  paragemPisoAtiva = true;
  paragemPisoAteMs = millis() + TEMPO_PARAGEM_PISO_MS;
  estabilizando = false;
  acaoElevador = 0;
  andarDestino = andar;
  paraMotor();
}

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

      if (antigo == HIGH && resetEstavel == LOW) {
        return true;
      }
    }
  }

  return false;
}

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
  return (portaEstavel == LOW);
}

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

        if (antigo == HIGH && botaoEstavel[i] == LOW) {
          int piso = mapaPedidoBotao[i];
          pedidos[piso] = true;
          pedidoRecenteOLED = piso + 1;
        }
      }
    }
  }
}

void limpaPedidosEDesligaLEDs() {
  for (int i = 0; i < 4; i++) {
    pedidos[i] = false;
    digitalWrite(ledsExt[i], LOW);
    digitalWrite(ledsInt[i], LOW);
  }
}

bool haPedidosPendentes() {
  for (int i = 0; i < 4; i++) {
    if (pedidos[i]) return true;
  }
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

// ================= OLED =================
void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

char dirSetaOLED(int acao) {
  if (acao == 1) return '^';
  if (acao == 2) return 'v';
  return '-';
}

uint8_t calculaModoEstadoOLED() {
  if (estadoSistema == NORMAL) {
    if (!portaPermissivaValida()) return OLED_EST_PORTA;
    if (paragemPisoAtiva) return OLED_EST_PARAGEM;
    if (estabilizando) return OLED_EST_ESTAB;
    return OLED_EST_NORMAL;
  }
  if (estadoSistema == ERRO_MOVENDO) return OLED_EST_ERRO_MOV;
  if (estadoSistema == ERRO_PARADO) return OLED_EST_ERRO_PAR;
  return OLED_EST_REARME;
}

OledSnapshot criaSnapshotAtual() {
  OledSnapshot s;
  s.andar = andar;
  s.andarDestino = andarDestino;
  s.sentidoAtual = sentidoAtual;
  s.acaoElevador = acaoElevador;
  s.pedidoRecente = pedidoRecenteOLED;
  s.modoEstado = calculaModoEstadoOLED();

  for (int i = 0; i < 4; i++) {
    s.pedidos[i] = pedidos[i];
  }

  return s;
}

bool snapshotsIguais(const OledSnapshot &a, const OledSnapshot &b) {
  if (a.andar != b.andar) return false;
  if (a.andarDestino != b.andarDestino) return false;
  if (a.sentidoAtual != b.sentidoAtual) return false;
  if (a.acaoElevador != b.acaoElevador) return false;
  if (a.pedidoRecente != b.pedidoRecente) return false;
  if (a.modoEstado != b.modoEstado) return false;

  for (int i = 0; i < 4; i++) {
    if (a.pedidos[i] != b.pedidos[i]) return false;
  }

  return true;
}

void renderFila(Adafruit_SH1106G &d, int xStart, int yStart, const OledSnapshot &s) {
  int x = xStart;
  int y = yStart;
  int cont = 0;

  if (s.andar >= 1 && s.andar <= 4) {
    if (s.sentidoAtual == 1) {
      for (int p = s.andar; p <= 4; p++) {
        if (s.pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
      for (int p = s.andar - 1; p >= 1; p--) {
        if (s.pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
    } else if (s.sentidoAtual == -1) {
      for (int p = s.andar; p >= 1; p--) {
        if (s.pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
      for (int p = s.andar + 1; p <= 4; p++) {
        if (s.pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
    } else {
      for (int p = 1; p <= 4; p++) {
        if (s.pedidos[p - 1]) {
          d.setCursor(x, y); d.print(p); d.print(' ');
          x += 12; cont++;
          if (x > 120) { x = 0; y += 10; }
        }
      }
    }
  } else {
    for (int p = 1; p <= 4; p++) {
      if (s.pedidos[p - 1]) {
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

void renderEstadoOLED(Adafruit_SH1106G &d, uint8_t modoEstado) {
  if (modoEstado == OLED_EST_PORTA) d.print("PORTA");
  else if (modoEstado == OLED_EST_PARAGEM) d.print("PARAGEM");
  else if (modoEstado == OLED_EST_ESTAB) d.print("ESTAB");
  else if (modoEstado == OLED_EST_ERRO_MOV) d.print("ERRO_MOV");
  else if (modoEstado == OLED_EST_ERRO_PAR) d.print("ERRO_PAR");
  else if (modoEstado == OLED_EST_REARME) d.print("REARME");
  else d.print("NORMAL");
}

void renderOneOLED(Adafruit_SH1106G &d, int idx, const OledSnapshot &s) {
  d.clearDisplay();
  d.setTextColor(SH110X_WHITE);

  d.setTextSize(1);
  d.setCursor(0, 0);
  d.print("OLED #");
  d.print(idx);

  d.setTextSize(2);
  d.setCursor(0, 12);
  d.print("P:");
  if (s.andar >= 1 && s.andar <= 4) d.print(s.andar);
  else d.print("?");

  d.setCursor(64, 12);
  d.print(dirSetaOLED(s.acaoElevador));

  d.setTextSize(1);
  d.setCursor(0, 34);
  d.print("Dest: ");
  if (s.andarDestino >= 1 && s.andarDestino <= 4) d.print(s.andarDestino);
  else d.print("-");

  d.setCursor(64, 34);
  d.print("Rec: ");
  if (s.pedidoRecente >= 1 && s.pedidoRecente <= 4) d.print(s.pedidoRecente);
  else d.print("-");

  d.setCursor(0, 46);
  d.print("Fila: ");
  renderFila(d, 30, 46, s);

  d.setCursor(0, 58);
  renderEstadoOLED(d, s.modoEstado);

  d.display();
}

void oledAtualizaUmCanal() {
  if (oledCanalAtual == 0) {
    tcaSelect(0);
    renderOneOLED(oled1, 1, oledAlvo);
  } else if (oledCanalAtual == 1) {
    tcaSelect(1);
    renderOneOLED(oled2, 2, oledAlvo);
  } else if (oledCanalAtual == 2) {
    tcaSelect(2);
    renderOneOLED(oled3, 3, oledAlvo);
  } else if (oledCanalAtual == 3) {
    tcaSelect(3);
    renderOneOLED(oled4, 4, oledAlvo);
  }

  oledCanalAtual++;

  if (oledCanalAtual >= 4) {
    oledCanalAtual = 0;
    oledUltimoCompleto = oledAlvo;
    oledCicloAtivo = false;
  }
}

void oledService() {
  OledSnapshot atual = criaSnapshotAtual();

  if (!oledCicloAtivo) {
    if (!snapshotsIguais(atual, oledUltimoCompleto)) {
      oledAlvo = atual;
      oledCicloAtivo = true;
      oledCanalAtual = 0;
    } else {
      return;
    }
  }

  if (millis() - lastOledMs < OLED_INTERVALO_MS) return;
  lastOledMs = millis();

  oledAtualizaUmCanal();
}

// ================= SETUP =================
void setup() {
  Serial.begin(9600);

  // botões exteriores + interiores
  for (int i = 0; i < NUM_BOTOES_PEDIDO; i++) {
    pinMode(botoesPedido[i], INPUT_PULLUP);
  }

  // reset
  pinMode(pinReset, INPUT_PULLUP);

  // LEDs exteriores
  for (int i = 0; i < 4; i++) {
    pinMode(ledsExt[i], OUTPUT);
    digitalWrite(ledsExt[i], LOW);
  }

  // LEDs interiores
  for (int i = 0; i < 4; i++) {
    pinMode(ledsInt[i], OUTPUT);
    digitalWrite(ledsInt[i], LOW);
  }

  // sensores Hall dos pisos
  for (int i = 0; i < 4; i++) {
    pinMode(sensoresHall[i], INPUT_PULLUP);
  }

  // sensor Hall da porta
  pinMode(pinPortaPermissiva, INPUT_PULLUP);

  // buzzer
  pinMode(pinBuzzer, OUTPUT);
  noTone(pinBuzzer);

  // motor
  pinMode(pinMotorIN1, OUTPUT);
  pinMode(pinMotorIN2, OUTPUT);
  pinMode(pinMotorIN3, OUTPUT);
  pinMode(pinMotorIN4, OUTPUT);
  desligaBobinasMotor();

  // I2C + OLEDs
  Wire.begin();

  tcaSelect(0); oled1.begin(OLED_ADDR, true);
  tcaSelect(1); oled2.begin(OLED_ADDR, true);
  tcaSelect(2); oled3.begin(OLED_ADDR, true);
  tcaSelect(3); oled4.begin(OLED_ADDR, true);

  tcaSelect(0); oled1.clearDisplay(); oled1.display();
  tcaSelect(1); oled2.clearDisplay(); oled2.display();
  tcaSelect(2); oled3.clearDisplay(); oled3.display();
  tcaSelect(3); oled4.clearDisplay(); oled4.display();

  // leitura inicial sensores piso
  for (int i = 0; i < 4; i++) {
    estadoSensor[i] = !digitalRead(sensoresHall[i]);
  }

  // identificar piso inicial
  andar = -1;
  for (int i = 0; i < 4; i++) {
    if (estadoSensor[i]) andar = i + 1;
  }

  // referenciação inicial se necessário
  if (andar == -1) {
    const unsigned long timeoutRefMs = 20000UL;
    unsigned long startRef = millis();

    motorDir = -1;
    stepRate = velocidadeDesce;

    while (andar == -1 && (millis() - startRef < timeoutRefMs)) {
      motorService();
      buzzerService();

      for (int i = 0; i < 4; i++) {
        estadoSensor[i] = !digitalRead(sensoresHall[i]);
      }

      if (estadoSensor[0]) andar = 1;
      if (estadoSensor[1]) andar = 2;
      if (estadoSensor[2]) andar = 3;
      if (estadoSensor[3]) andar = 4;
    }

    paraMotor();

    if (andar == -1) {
      estadoSistema = ERRO_PARADO;
      iniciaAlarmeContinuo();
    }
  }

  andarDestino = andar;
  acaoElevador = 0;
  sentidoAtual = 0;

  for (int i = 0; i < 4; i++) {
    estadoSensorAnt[i] = estadoSensor[i];
  }

  unsigned long agora = millis();

  // init debounce botões
  for (int i = 0; i < NUM_BOTOES_PEDIDO; i++) {
    bool raw = digitalRead(botoesPedido[i]);
    botaoRawAnt[i] = raw;
    botaoEstavel[i] = raw;
    botaoUltMudancaMs[i] = agora;
  }

  // init reset
  resetRawAnt = digitalRead(pinReset);
  resetEstavel = resetRawAnt;
  resetUltMudancaMs = agora;

  // init porta
  portaRawAnt = digitalRead(pinPortaPermissiva);
  portaEstavel = portaRawAnt;
  portaUltMudancaMs = agora;

  // snapshot inicial
  oledUltimoCompleto = criaSnapshotAtual();
  oledAlvo = oledUltimoCompleto;

  // desenho inicial completo
  tcaSelect(0); renderOneOLED(oled1, 1, oledUltimoCompleto);
  tcaSelect(1); renderOneOLED(oled2, 2, oledUltimoCompleto);
  tcaSelect(2); renderOneOLED(oled3, 3, oledUltimoCompleto);
  tcaSelect(3); renderOneOLED(oled4, 4, oledUltimoCompleto);

  lastOledMs = millis();
}

// ================= LOOP =================
void loop() {
  motorService();
  buzzerService();
  oledService();

  if (millis() - lastLogicMs < 10) return;
  lastLogicMs = millis();

  debouncePortaPermissiva();

  if (resetFoiPremido()) {
    iniciaRearme();
  }

  // ler sensores piso
  for (int i = 0; i < 4; i++) {
    estadoSensor[i] = !digitalRead(sensoresHall[i]);
  }

  if (estadoSistema == NORMAL) {
    debounceBotoesERegistaPedidos();
  }

  if (estadoSensor[0] && !estadoSensorAnt[0]) processaChegadaPiso(1, 0);
  if (estadoSensor[1] && !estadoSensorAnt[1]) processaChegadaPiso(2, 1);
  if (estadoSensor[2] && !estadoSensorAnt[2]) processaChegadaPiso(3, 2);
  if (estadoSensor[3] && !estadoSensorAnt[3]) processaChegadaPiso(4, 3);

  atendePedidoNoPisoAtualSeExistir();

  if (estabilizando && millis() >= estabilizaAteMs) {
    estabilizando = false;
  }

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

  // guardar estados anteriores sensores
  for (int i = 0; i < 4; i++) {
    estadoSensorAnt[i] = estadoSensor[i];
  }
}

