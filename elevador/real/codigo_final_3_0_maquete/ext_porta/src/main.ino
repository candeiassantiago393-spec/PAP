/*
  ELEVADOR 4 PISOS — L298N + PORTA + pedidos EXTERIORES
  Sem OLED, sem botões interiores.

  Pinagem Mega (bancada):
    Botões ext: D2-D5   LEDs ext: A0-A3
    Hall pisos 1-4: D7, D8, D11, D12  (LOW = íman)
    Hall porta: D22  (LOW = fechada, HIGH = aberta)
    Reset: D10  (D7 ocupado pelo Hall piso 1)
    Buzzer: D6
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

#define pinBuzzer 6
#define pinReset 10

#define pinSensor1 7
#define pinSensor2 8
#define pinSensor3 11
#define pinSensor4 12

#define pinPortaPermissiva 22

#define pinLedExt1 A0
#define pinLedExt2 A1
#define pinLedExt3 A2
#define pinLedExt4 A3

const int ledsExt[4] = {pinLedExt1, pinLedExt2, pinLedExt3, pinLedExt4};
const int sensoresHall[4] = {pinSensor1, pinSensor2, pinSensor3, pinSensor4};
const int botoesExt[4] = {pinBotaoExt1, pinBotaoExt2, pinBotaoExt3, pinBotaoExt4};

#define NUM_BOTOES 4
#define DEBOUNCE_MS 50UL
#define HALL_CONFIRMA_MS 120UL
#define TEMPO_ESTABILIZA_MS 300UL
#define TEMPO_PARAGEM_PISO_MS 10000UL
#define TEMPO_REARME_ESPERA_MS 3000UL   // teste (produção: 10000)
#define TIMEOUT_MOV_MS 45000UL

#define MOTOR_INVERTIDO 1

bool pedidos[4] = {false, false, false, false};
bool estadoSensor[4];
bool hallConfirmado[4];
bool hallConfirmadoAnt[4];
bool pisoHallProcessado[4];
unsigned long hallAtivoDesdeMs[4];

int acaoElevador = 0;
int andar = -1;
int andarDestino = -1;
int sentidoAtual = 0;

volatile int stepRate = 0;
unsigned long lastStepMicros = 0;
int motorDir = 0;
int faseMotor = 0;

bool beepAtivo = false;
unsigned long beepAteMs = 0;
bool alarmeAtivo = false;

unsigned long lastLogicMs = 0;

bool estabilizando = false;
unsigned long estabilizaAteMs = 0;
bool paragemPisoAtiva = false;
unsigned long paragemPisoAteMs = 0;

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

bool botaoRawAnt[NUM_BOTOES];
bool botaoEstavel[NUM_BOTOES];
unsigned long botaoUltMudancaMs[NUM_BOTOES];

bool resetRawAnt = HIGH;
bool resetEstavel = HIGH;
unsigned long resetUltMudancaMs = 0;

bool portaRawAnt = HIGH;
bool portaEstavel = HIGH;
unsigned long portaUltMudancaMs = 0;

int dirFisico(int dirLogico) {
  return MOTOR_INVERTIDO ? -dirLogico : dirLogico;
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

void desligaBobinasMotor() {
  digitalWrite(pinMotorIN1, LOW);
  digitalWrite(pinMotorIN2, LOW);
  digitalWrite(pinMotorIN3, LOW);
  digitalWrite(pinMotorIN4, LOW);
}

void paraMotor() {
  stepRate = 0;
  motorDir = 0;
  // Piso 1: tábua de apoio — bobinas desligadas (motor "relaxado")
  if (andar == 1) desligaBobinasMotor();
  else aplicaFaseMotor(faseMotor);
}

void arrancaMotorLogico(int dirLogico) {
  if (dirLogico == 0) {
    paraMotor();
    return;
  }
  motorDir = dirFisico(dirLogico);
  stepRate = (dirLogico > 0) ? velocidadeSobe : velocidadeDesce;
}

void motorService() {
  if (stepRate <= 0 || motorDir == 0) return;
  unsigned long now = micros();
  unsigned long intervalo = 1000000UL / (unsigned long)stepRate;
  if (now - lastStepMicros >= intervalo) {
    lastStepMicros = now;
    if (motorDir > 0) faseMotor = (faseMotor + 1) & 0x03;
    else faseMotor = (faseMotor + 3) & 0x03;
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
  Serial.println(andar);
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
      if (antigo == HIGH && resetEstavel == LOW) return true;
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
          if (!pedidos[i]) {
            pedidos[i] = true;
            beepCurto();
            Serial.print(F(">> Pedido PISO "));
            Serial.println(i + 1);
          }
        }
      }
    }
  }
}

void limpaPedidosEDesligaLEDs() {
  for (int i = 0; i < 4; i++) {
    pedidos[i] = false;
    digitalWrite(ledsExt[i], LOW);
  }
}

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
  if (andarDestino > andar) { acaoElevador = 1; sentidoAtual = 1; }
  else if (andarDestino < andar) { acaoElevador = 2; sentidoAtual = -1; }
  else if (!movimentoAtivo) acaoElevador = 0;
}

void atendePedidoNoPisoAtualSeExistir() {
  if (estadoSistema != NORMAL) return;
  if (andar < 1 || andar > 4) return;
  if (estabilizando || paragemPisoAtiva) return;
  if (acaoElevador != 0) return;
  int idx = andar - 1;
  if (pedidos[idx]) {
    beepCurto();
    pedidos[idx] = false;
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
  Serial.println(F(">> ERRO MOVENDO"));
}

void fixaErroParado() {
  estadoSistema = ERRO_PARADO;
  paragemPisoAtiva = false;
  paraMotor();
  acaoElevador = 0;
  Serial.println(F(">> ERRO PARADO"));
}

void iniciaRearme() {
  estadoSistema = REARME_ATIVO;
  limpaPedidosEDesligaLEDs();
  iniciaAlarmeContinuo();
  estabilizando = false;
  paragemPisoAtiva = false;
  Serial.println(F(">> REARME"));
  if (acaoElevador == 1 || acaoElevador == 2) faseRearme = REARME_ATE_PISO_VALIDO;
  else {
    faseRearme = REARME_PAUSA_2S;
    faseAteMs = millis() + 2000UL;
    paraMotor();
    acaoElevador = 0;
  }
}

void processaChegadaPiso(int piso, int idxPedido) {
  andar = piso;
  movimentoAtivo = false;
  inicioMovMs = millis();

  if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_ATE_PISO_VALIDO) {
      paraMotor(); acaoElevador = 0;
      faseRearme = REARME_PAUSA_2S; faseAteMs = millis() + 2000UL;
      return;
    }
    if (faseRearme == REARME_IR_PISO1 && piso == 1) {
      paraMotor(); acaoElevador = 0;
      faseRearme = REARME_ESPERA_10S; faseAteMs = millis() + TEMPO_REARME_ESPERA_MS;
      return;
    }
    return;
  }
  if (estadoSistema == ERRO_MOVENDO) { fixaErroParado(); return; }
  if (estadoSistema == ERRO_PARADO) return;
  bool tinhaPedido = pedidos[idxPedido];
  if (tinhaPedido) beepCurto();
  pedidos[idxPedido] = false;
  if (tinhaPedido) iniciaParagemPisoPedido();
  else if (andarDestino == piso) iniciaEstabilizacao();
}

void lerSensoresPiso() {
  unsigned long agora = millis();
  for (int i = 0; i < 4; i++) {
    bool raw = (digitalRead(sensoresHall[i]) == LOW);
    estadoSensor[i] = raw;
    if (raw) {
      if (hallAtivoDesdeMs[i] == 0) hallAtivoDesdeMs[i] = agora;
      hallConfirmado[i] = ((agora - hallAtivoDesdeMs[i]) >= HALL_CONFIRMA_MS);
    } else {
      hallAtivoDesdeMs[i] = 0;
      hallConfirmado[i] = false;
    }
  }
}

void verificaHallPiso() {
  for (int i = 0; i < 4; i++) {
    if (!hallConfirmado[i]) {
      pisoHallProcessado[i] = false;
      continue;
    }
    if (pisoHallProcessado[i]) continue;

    int piso = i + 1;
    bool borda = hallConfirmado[i] && !hallConfirmadoAnt[i];

    if (estadoSistema == REARME_ATIVO || estadoSistema == ERRO_MOVENDO) {
      if (borda) {
        pisoHallProcessado[i] = true;
        processaChegadaPiso(piso, i);
      }
      continue;
    }

    if (estadoSistema != NORMAL) continue;

    if (acaoElevador != 0) {
      bool tinhaPedido = pedidos[i];
      bool ehDestino = (piso == andarDestino);
      if (tinhaPedido || ehDestino) {
        pisoHallProcessado[i] = true;
        processaChegadaPiso(piso, i);
        continue;
      }
      if (borda && andar != piso) {
        andar = piso;
        inicioMovMs = millis();
        Serial.print(F(">> Passagem piso "));
        Serial.println(piso);
      }
    } else if (borda) {
      pisoHallProcessado[i] = true;
      processaChegadaPiso(piso, i);
    }
  }

  for (int i = 0; i < 4; i++) hallConfirmadoAnt[i] = hallConfirmado[i];
}

void setup() {
  Serial.begin(9600);
  delay(400);

  for (int i = 0; i < NUM_BOTOES; i++) pinMode(botoesExt[i], INPUT_PULLUP);
  pinMode(pinReset, INPUT_PULLUP);
  pinMode(pinPortaPermissiva, INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    pinMode(ledsExt[i], OUTPUT);
    digitalWrite(ledsExt[i], LOW);
    pinMode(sensoresHall[i], INPUT_PULLUP);
  }

  pinMode(pinBuzzer, OUTPUT);
  noTone(pinBuzzer);

  pinMode(pinMotorIN1, OUTPUT);
  pinMode(pinMotorIN2, OUTPUT);
  pinMode(pinMotorIN3, OUTPUT);
  pinMode(pinMotorIN4, OUTPUT);
  desligaBobinasMotor();

  Serial.println(F("=== ELEVADOR EXT + PORTA ==="));
  Serial.println(F("Hall: D7,D8,D11,D12 | Porta: D22 | Reset: D10"));

  lerSensoresPiso();
  delay(HALL_CONFIRMA_MS + 20);
  lerSensoresPiso();
  andar = -1;
  for (int i = 0; i < 4; i++) if (hallConfirmado[i]) andar = i + 1;

  if (andar == -1) {
    Serial.println(F("Referencia: desce ate detetar piso..."));
    unsigned long startRef = millis();
    arrancaMotorLogico(-1);
    while (andar == -1 && (millis() - startRef < 20000UL)) {
      motorService();
      buzzerService();
      debouncePortaPermissiva();
      lerSensoresPiso();
      if (hallConfirmado[0]) andar = 1;
      else if (hallConfirmado[1]) andar = 2;
      else if (hallConfirmado[2]) andar = 3;
      else if (hallConfirmado[3]) andar = 4;
    }
    paraMotor();
    if (andar == -1) {
      estadoSistema = ERRO_PARADO;
      iniciaAlarmeContinuo();
      Serial.println(F("ERRO: referencia"));
    } else {
      Serial.print(F("Referencia OK — piso ")); Serial.println(andar);
    }
  } else {
    Serial.print(F("Arranque no piso ")); Serial.println(andar);
  }

  andarDestino = andar;
  acaoElevador = 0;
  sentidoAtual = 0;
  for (int i = 0; i < 4; i++) {
    hallAtivoDesdeMs[i] = 0;
    pisoHallProcessado[i] = false;
    hallConfirmadoAnt[i] = hallConfirmado[i];
  }

  unsigned long agora = millis();
  for (int i = 0; i < NUM_BOTOES; i++) {
    bool raw = digitalRead(botoesExt[i]);
    botaoRawAnt[i] = raw; botaoEstavel[i] = raw; botaoUltMudancaMs[i] = agora;
  }
  resetRawAnt = digitalRead(pinReset);
  resetEstavel = resetRawAnt;
  resetUltMudancaMs = agora;
  portaRawAnt = digitalRead(pinPortaPermissiva);
  portaEstavel = portaRawAnt;
  portaUltMudancaMs = agora;
}

void loop() {
  motorService();
  buzzerService();

  if (millis() - lastLogicMs < 10) return;
  lastLogicMs = millis();

  debouncePortaPermissiva();
  if (resetFoiPremido()) iniciaRearme();

  lerSensoresPiso();
  if (estadoSistema == NORMAL) debounceBotoesERegistaPedidos();

  verificaHallPiso();

  atendePedidoNoPisoAtualSeExistir();

  if (estabilizando && millis() >= estabilizaAteMs) {
    estabilizando = false;
    selecionaDestinoEAcao();
  }

  if (paragemPisoAtiva) {
    if ((millis() >= paragemPisoAteMs) && portaPermissivaValida()) {
      paragemPisoAtiva = false;
      Serial.println(F(">> Fim paragem — continua"));
      selecionaDestinoEAcao();
    }
  }

  if (estadoSistema == NORMAL) {
    if (!estabilizando && !paragemPisoAtiva && stepRate == 0 && portaPermissivaValida()) {
      selecionaDestinoEAcao();
    }

    bool movimentoCmd = !estabilizando && !paragemPisoAtiva &&
                        portaPermissivaValida() &&
                        (acaoElevador == 1 || acaoElevador == 2);

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
      entraErroMovendo();
      movimentoAtivo = false;
    }
  }

  if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_PAUSA_2S && millis() >= faseAteMs) {
      if (andar == 1) {
        faseRearme = REARME_ESPERA_10S;
        faseAteMs = millis() + TEMPO_REARME_ESPERA_MS;
      } else {
        faseRearme = REARME_IR_PISO1;
        rearmeMoveStartMs = millis();
        andarDestino = 1;
        acaoElevador = 2;
        sentidoAtual = -1;
      }
    }
    if (faseRearme == REARME_IR_PISO1 && millis() - rearmeMoveStartMs >= TIMEOUT_MOV_MS) {
      faseRearme = REARME_NONE;
      entraErroMovendo();
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
      Serial.println(F(">> REARME OK"));
    }
  }

  if (estadoSistema == ERRO_PARADO) {
    paraMotor();
  } else if (estadoSistema == REARME_ATIVO) {
    if (faseRearme == REARME_PAUSA_2S || faseRearme == REARME_ESPERA_10S) paraMotor();
    else if (acaoElevador == 1) arrancaMotorLogico(+1);
    else if (acaoElevador == 2) arrancaMotorLogico(-1);
    else paraMotor();
  } else if (estabilizando || paragemPisoAtiva || !portaPermissivaValida()) {
    paraMotor();
  } else if (acaoElevador == 1) {
    arrancaMotorLogico(+1);
  } else if (acaoElevador == 2) {
    arrancaMotorLogico(-1);
  } else {
    paraMotor();
  }

  if (estadoSistema == NORMAL) {
    for (int i = 0; i < 4; i++) digitalWrite(ledsExt[i], pedidos[i] ? HIGH : LOW);
  } else {
    for (int i = 0; i < 4; i++) digitalWrite(ledsExt[i], LOW);
  }

}
