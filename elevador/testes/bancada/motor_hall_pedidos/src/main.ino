/*
  Teste bancada — pedidos EXTERIORES + motor L298N + sensores Hall
  Elevador 4 pisos.

  Arranque: assume PISO 1 (cabine ja la) — motor parado.
  Depois: aceita pedidos exteriores (botao -> LED ON + fila),
          para 5s no piso, LED OFF ao atender, continua fila SCAN.

  Pinagem Mega:
    Botões ext: D2-D5   LEDs ext: A0-A3
    Hall: D7=piso1, D8=piso2, D11=piso3, D12=piso4 (LOW = íman)
    L298N: IN1=D9 IN2=D33 IN3=D31 IN4=D32
*/

#include <Arduino.h>

#define pinMotorIN1 9
#define pinMotorIN2 33
#define pinMotorIN3 31
#define pinMotorIN4 32

#define velocidadeSobe 80
#define velocidadeDesce 80

#define pinBotaoExt1 2
#define pinBotaoExt2 3
#define pinBotaoExt3 4
#define pinBotaoExt4 5

#define pinLedExt1 A0
#define pinLedExt2 A1
#define pinLedExt3 A2
#define pinLedExt4 A3

#define pinSensor1 7
#define pinSensor2 8
#define pinSensor3 11
#define pinSensor4 12

const int botoesExt[4] = {pinBotaoExt1, pinBotaoExt2, pinBotaoExt3, pinBotaoExt4};
const int ledsExt[4] = {pinLedExt1, pinLedExt2, pinLedExt3, pinLedExt4};
const int sensoresHall[4] = {pinSensor1, pinSensor2, pinSensor3, pinSensor4};

#define NUM_BOTOES 4
#define PISO_MIN 1
#define PISO_MAX 4
#define DEBOUNCE_MS 50UL
#define HALL_CONFIRMA_MS 120UL
#define TEMPO_ESTABILIZA_MS 300UL
#define TEMPO_PARAGEM_PISO_MS 5000UL
#define TIMEOUT_MOV_MS 45000UL

// Se SOBE quando devia DESCER (ou vice-versa), muda entre 0 e 1.
#define MOTOR_INVERTIDO 1

bool pedidos[4] = {false, false, false, false};
bool hallAtivo[4];
bool hallConfirmado[4];
unsigned long hallAtivoDesdeMs[4];
bool pisoHallProcessado[4];

int andar = -1;
int andarDestino = PISO_MIN;
int acaoElevador = 0;   // 0=parado 1=subir 2=descer
int sentidoAtual = 0;   // 0=idle 1=subir -1=descer

volatile int stepRate = 0;
unsigned long lastStepMicros = 0;
int motorDir = 0;
int faseMotor = 0;

bool estabilizando = false;
unsigned long estabilizaAteMs = 0;
bool paragemPisoAtiva = false;
unsigned long paragemPisoAteMs = 0;

bool movimentoAtivo = false;
unsigned long inicioMovMs = 0;
bool erroParado = false;
bool pronto = false;

bool botaoRawAnt[NUM_BOTOES];
bool botaoEstavel[NUM_BOTOES];
unsigned long botaoUltMudancaMs[NUM_BOTOES];

unsigned long lastLogicMs = 0;
unsigned long lastSerialMs = 0;

void selecionaDestinoEAcao();
void atualizarLeds();
void lerHall();
void paraMotor();
void processaChegadaPiso(int piso, int idx);
void verificaHallParagem();

int dirFisico(int dirLogico) {
  return MOTOR_INVERTIDO ? -dirLogico : dirLogico;
}

void arrancaMotorLogico(int dirLogico) {
  if (dirLogico == 0) {
    paraMotor();
    return;
  }
  motorDir = dirFisico(dirLogico);
  stepRate = (dirLogico > 0) ? velocidadeSobe : velocidadeDesce;
}

void testeLedsArranque() {
  Serial.println(F("Teste LEDs A0-A3 (cada um 300ms)..."));
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      digitalWrite(ledsExt[j], (j == i) ? HIGH : LOW);
    }
    delay(300);
  }
  for (int i = 0; i < 4; i++) digitalWrite(ledsExt[i], LOW);
  Serial.println(F("Teste LEDs fim"));
}

void diagnosticoArranque() {
  lerHall();
  Serial.print(F("Hall D7,D8,D11,D12 (1=activo): "));
  for (int i = 0; i < 4; i++) Serial.print(hallAtivo[i] ? '1' : '0');
  Serial.println();

  Serial.print(F("Botoes D2-D5 (0=premido): "));
  for (int i = 0; i < 4; i++) {
    Serial.print(digitalRead(botoesExt[i]) == LOW ? '0' : '1');
  }
  Serial.println();
}

void registaPedido(int idx) {
  if (pedidos[idx]) return;
  pedidos[idx] = true;
  atualizarLeds();
  Serial.print(F(">> Pedido PISO "));
  Serial.print(idx + 1);
  Serial.println(F(" — LED ON"));
}

void limpaPedidoAtendido(int idx) {
  if (!pedidos[idx]) return;
  pedidos[idx] = false;
  atualizarLeds();
  Serial.print(F(">> PISO "));
  Serial.print(idx + 1);
  Serial.println(F(" atendido — LED OFF"));
}

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

void paraMotor() {
  stepRate = 0;
  motorDir = 0;
  aplicaFaseMotor(faseMotor);
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

void lerHall() {
  unsigned long agora = millis();
  for (int i = 0; i < 4; i++) {
    bool raw = (digitalRead(sensoresHall[i]) == LOW);
    hallAtivo[i] = raw;

    if (raw) {
      if (hallAtivoDesdeMs[i] == 0) hallAtivoDesdeMs[i] = agora;
      hallConfirmado[i] = ((agora - hallAtivoDesdeMs[i]) >= HALL_CONFIRMA_MS);
    } else {
      hallAtivoDesdeMs[i] = 0;
      hallConfirmado[i] = false;
    }
  }
}

void verificaHallParagem() {
  if (!pronto || paragemPisoAtiva || estabilizando) return;

  for (int i = 0; i < 4; i++) {
    if (!hallConfirmado[i]) {
      pisoHallProcessado[i] = false;
      continue;
    }

    if (pisoHallProcessado[i]) continue;

    int piso = i + 1;
    bool tinhaPedido = pedidos[i];
    bool ehDestino = (piso == andarDestino);
    bool regressaPiso1 = (piso == PISO_MIN && !haPedidosPendentes() && acaoElevador == 2);

    if (acaoElevador != 0 && (tinhaPedido || ehDestino || regressaPiso1)) {
      pisoHallProcessado[i] = true;
      processaChegadaPiso(piso, i);
      return;
    }

    if (acaoElevador != 0 && andar != piso) {
      andar = piso;
      inicioMovMs = millis();
      Serial.print(F(">> Passagem piso "));
      Serial.println(piso);
    }
  }
}

int pisoDoHallAtivo() {
  for (int i = 0; i < 4; i++) {
    if (hallAtivo[i]) return i + 1;
  }
  return -1;
}

void iniciaEstabilizacao() {
  estabilizando = true;
  estabilizaAteMs = millis() + TEMPO_ESTABILIZA_MS;
  acaoElevador = 0;
  paraMotor();
}

void iniciaParagemPisoPedido() {
  paragemPisoAtiva = true;
  paragemPisoAteMs = millis() + TEMPO_PARAGEM_PISO_MS;
  estabilizando = false;
  acaoElevador = 0;
  andarDestino = andar;
  paraMotor();
  Serial.print(F(">> PARAGEM piso "));
  Serial.print(andar);
  Serial.println(F(" (5s)"));
}

void debounceBotoesERegistaPedidos() {
  if (erroParado) return;
  if (!pronto) return;

  unsigned long agora = millis();

  for (int i = 0; i < NUM_BOTOES; i++) {
    bool raw = digitalRead(botoesExt[i]);

    if (raw != botaoRawAnt[i]) {
      botaoRawAnt[i] = raw;
      botaoUltMudancaMs[i] = agora;
    }

    if ((agora - botaoUltMudancaMs[i]) >= DEBOUNCE_MS) {
      if (raw != botaoEstavel[i]) {
        bool antigo = botaoEstavel[i];
        botaoEstavel[i] = raw;

        if (antigo == HIGH && botaoEstavel[i] == LOW) {
          registaPedido(i);

          // Recalcula destino (mesmo em movimento) — novo pedido pode mudar a fila SCAN
          if (!paragemPisoAtiva && !estabilizando) {
            selecionaDestinoEAcao();
          }
        }
      }
    }
  }
}

bool haPedidosPendentes() {
  for (int i = 0; i < 4; i++) {
    if (pedidos[i]) return true;
  }
  return false;
}

int proximoPedidoAcima(int pisoAtual) {
  for (int p = pisoAtual; p <= PISO_MAX; p++) {
    if (pedidos[p - 1]) return p;
  }
  return -1;
}

int proximoPedidoAbaixo(int pisoAtual) {
  for (int p = pisoAtual - 1; p >= PISO_MIN; p--) {
    if (pedidos[p - 1]) return p;
  }
  return -1;
}

void selecionaDestinoEAcao() {
  if (!pronto || erroParado) return;
  if (andar < PISO_MIN || andar > PISO_MAX) return;
  if (paragemPisoAtiva || estabilizando) return;

  if (!haPedidosPendentes()) {
    if (hallConfirmado[0]) {
      andar = PISO_MIN;
      andarDestino = PISO_MIN;
      acaoElevador = 0;
      sentidoAtual = 0;
      return;
    }
    if (andar > PISO_MIN) {
      andarDestino = PISO_MIN;
      acaoElevador = 2;
      sentidoAtual = -1;
      Serial.println(F(">> Fila vazia — regressa piso 1"));
      return;
    }
    andarDestino = PISO_MIN;
    acaoElevador = 0;
    sentidoAtual = 0;
    return;
  }

  int dest = -1;

  if (sentidoAtual == 1) {
    dest = proximoPedidoAcima(andar);
    if (dest == -1) {
      dest = proximoPedidoAbaixo(andar);
      if (dest != -1) sentidoAtual = -1;
    }
  } else if (sentidoAtual == -1) {
    dest = proximoPedidoAbaixo(andar);
    if (dest == -1) {
      dest = proximoPedidoAcima(andar);
      if (dest != -1) sentidoAtual = 1;
    }
  } else {
    dest = proximoPedidoAcima(andar);
    if (dest == -1) dest = proximoPedidoAbaixo(andar);
    if (dest != -1) {
      sentidoAtual = (dest > andar) ? 1 : -1;
    }
  }

  if (dest == -1) {
    andarDestino = andar;
    acaoElevador = 0;
    return;
  }

  andarDestino = dest;

  if (andarDestino > andar) {
    acaoElevador = 1;
    sentidoAtual = 1;
  } else if (andarDestino < andar) {
    acaoElevador = 2;
    sentidoAtual = -1;
  } else if (!movimentoAtivo) {
    acaoElevador = 0;
  }
}

void atendePedidoNoPisoAtual() {
  if (!pronto || erroParado) return;
  if (andar < PISO_MIN || andar > PISO_MAX) return;
  if (paragemPisoAtiva || estabilizando) return;
  if (acaoElevador != 0) return;

  int idx = andar - 1;
  if (pedidos[idx]) {
    limpaPedidoAtendido(idx);
    iniciaParagemPisoPedido();
  }
}

void processaChegadaPiso(int piso, int idx) {
  andar = piso;
  movimentoAtivo = false;
  inicioMovMs = millis();

  Serial.print(F(">> Hall confirma piso "));
  Serial.println(piso);

  if (pedidos[idx]) {
    limpaPedidoAtendido(idx);
    iniciaParagemPisoPedido();
    return;
  }

  if (piso == andarDestino || (piso == PISO_MIN && !haPedidosPendentes())) {
    iniciaEstabilizacao();
    if (piso == PISO_MIN) {
      Serial.println(F(">> Piso 1 — aguarda pedidos"));
    }
  }
}

void atualizarLeds() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledsExt[i], pedidos[i] ? HIGH : LOW);
  }
}

void aplicarComandoMotor() {
  if (erroParado || !pronto || paragemPisoAtiva || estabilizando || acaoElevador == 0) {
    paraMotor();
    return;
  }

  if (acaoElevador == 1) {
    arrancaMotorLogico(+1);
  } else if (acaoElevador == 2) {
    arrancaMotorLogico(-1);
  }
}

void imprimirSerial() {
  Serial.print(F("Andar="));
  if (andar >= PISO_MIN) Serial.print(andar);
  else Serial.print('?');

  Serial.print(F(" Dest="));
  if (andarDestino >= PISO_MIN) Serial.print(andarDestino);
  else Serial.print('-');

  Serial.print(F(" Sent="));
  if (sentidoAtual == 1) Serial.print('+');
  else if (sentidoAtual == -1) Serial.print('-');
  else Serial.print('0');

  Serial.print(F(" Acao="));
  if (acaoElevador == 1) Serial.print('^');
  else if (acaoElevador == 2) Serial.print('v');
  else Serial.print('.');

  Serial.print(F(" Hall:"));
  for (int i = 0; i < 4; i++) Serial.print(hallAtivo[i] ? '1' : '0');

  Serial.print(F(" Fila:"));
  for (int i = 0; i < 4; i++) Serial.print(pedidos[i] ? '1' : '0');

  if (!pronto) Serial.print(F(" [INIT]"));
  if (paragemPisoAtiva) Serial.print(F(" [PARAGEM]"));
  if (estabilizando) Serial.print(F(" [ESTAB]"));
  if (erroParado) Serial.print(F(" [ERRO]"));
  Serial.println();
}

void inicializaPiso1() {
  paraMotor();
  lerHall();

  int p = pisoDoHallAtivo();
  if (p > 0) {
    andar = p;
    Serial.print(F("Arranque: Hall confirma piso "));
    Serial.println(andar);
  } else {
    andar = PISO_MIN;
    Serial.println(F("Arranque: assume piso 1 — motor parado"));
  }

  pronto = true;
}

void setup() {
  Serial.begin(9600);
  delay(400);

  for (int i = 0; i < 4; i++) {
    pinMode(botoesExt[i], INPUT_PULLUP);
    pinMode(ledsExt[i], OUTPUT);
    digitalWrite(ledsExt[i], LOW);
    pinMode(sensoresHall[i], INPUT_PULLUP);
  }

  pinMode(pinMotorIN1, OUTPUT);
  pinMode(pinMotorIN2, OUTPUT);
  pinMode(pinMotorIN3, OUTPUT);
  pinMode(pinMotorIN4, OUTPUT);
  paraMotor();

  Serial.println();
  Serial.println(F("=== ELEVADOR BANCADA ==="));
  Serial.println(F("Arranque: PISO 1 (sem mover) | SCAN | Para 5s | Regresso piso 1"));
  Serial.print(F("Motor invertido="));
  Serial.println(MOTOR_INVERTIDO ? F("SIM") : F("NAO"));
  Serial.println(F("Botoes D2-D5 + LEDs A0-A3"));
  Serial.println(F("--------------------------------------"));

  testeLedsArranque();
  diagnosticoArranque();

  unsigned long agora = millis();
  for (int i = 0; i < NUM_BOTOES; i++) {
    bool raw = digitalRead(botoesExt[i]);
    botaoRawAnt[i] = raw;
    botaoEstavel[i] = raw;
    botaoUltMudancaMs[i] = agora;
  }

  inicializaPiso1();

  andarDestino = 1;
  acaoElevador = 0;
  sentidoAtual = 0;

  lerHall();
  for (int i = 0; i < 4; i++) {
    hallAtivoDesdeMs[i] = 0;
    hallConfirmado[i] = false;
    pisoHallProcessado[i] = false;
  }

  Serial.println(F("Pronto — aceita pedidos exteriores"));
  imprimirSerial();
}

void loop() {
  motorService();

  if (millis() - lastLogicMs < 10) return;
  lastLogicMs = millis();

  lerHall();
  debounceBotoesERegistaPedidos();
  verificaHallParagem();

  atendePedidoNoPisoAtual();

  if (estabilizando && millis() >= estabilizaAteMs) {
    estabilizando = false;
    acaoElevador = 0;
    sentidoAtual = 0;
    selecionaDestinoEAcao();
  }

  if (paragemPisoAtiva && millis() >= paragemPisoAteMs) {
    paragemPisoAtiva = false;
    Serial.println(F(">> Fim paragem — continua fila"));
    selecionaDestinoEAcao();
  }

  if (pronto && !erroParado && !paragemPisoAtiva && !estabilizando && stepRate == 0) {
    selecionaDestinoEAcao();
  }

  if (pronto && !erroParado) {
    bool movimentoCmd = (acaoElevador == 1 || acaoElevador == 2) &&
                        !paragemPisoAtiva && !estabilizando;

    if (movimentoCmd && !movimentoAtivo) {
      movimentoAtivo = true;
      inicioMovMs = millis();
      Serial.print(F(">> Move "));
      Serial.print(acaoElevador == 1 ? F("SOBE") : F("DESCE"));
      Serial.print(F(" -> piso "));
      Serial.println(andarDestino);
    } else if (!movimentoCmd) {
      movimentoAtivo = false;
    }

    if (movimentoAtivo && (millis() - inicioMovMs >= TIMEOUT_MOV_MS)) {
      Serial.println(F("AVISO: timeout — Hall nao detetou piso destino"));
      Serial.println(F("  Verifica imans D7,D8,D11,D12 — recontinua"));
      inicioMovMs = millis();
      selecionaDestinoEAcao();
    }
  }

  aplicarComandoMotor();
  atualizarLeds();

  if (millis() - lastSerialMs >= 500) {
    lastSerialMs = millis();
    imprimirSerial();
  }
}
