/*
  Teste bancada — 4 sensores Hall + motor L298N

  Comportamento:
    - Arranca a SUBIR (mesmo parado no piso 1 — nao reporta Hall ja activo)
    - Cada sensor detectado (borda: entra na zona) -> informa no Serial
    - Ao detectar PISO 4 -> comeca a DESCER
    - Na descida, informa cada piso que passa
    - Ao detectar PISO 1 na descida -> para

  Pinagem maquete:
    Piso 1=D7  Piso 2=D8  Piso 3=D11  Piso 4=D12  (LOW=iman)
    L298N: IN1=D9 IN2=D33 IN3=D31 IN4=D32
*/

#include <Arduino.h>

#define pinMotorIN1 9
#define pinMotorIN2 33
#define pinMotorIN3 31
#define pinMotorIN4 32

const int sensoresHall[4] = {7, 8, 11, 12};

#define velocidadeSobe 80
#define velocidadeDesce 80
#define HALL_CONFIRMA_MS 120UL
#define TIMEOUT_MOV_MS 45000UL

#define MOTOR_INVERTIDO 1

enum Sentido { SUBINDO, DESCENDO, PARADO };

bool hallAtivo[4];
bool hallConfirmado[4];
bool hallConfirmadoAnt[4];
unsigned long hallAtivoDesdeMs[4];

volatile int stepRate = 0;
unsigned long lastStepMicros = 0;
int motorDir = 0;
int faseMotor = 0;

Sentido sentido = SUBINDO;
unsigned long movimentoDesdeMs = 0;

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

void paraMotor() {
  stepRate = 0;
  motorDir = 0;
  aplicaFaseMotor(faseMotor);
}

void arrancaSentido(Sentido s) {
  sentido = s;
  if (s == SUBINDO) {
    motorDir = dirFisico(+1);
    stepRate = velocidadeSobe;
    Serial.println(F(">> Motor SOBE"));
  } else if (s == DESCENDO) {
    motorDir = dirFisico(-1);
    stepRate = velocidadeDesce;
    Serial.println(F(">> Motor DESCE"));
  } else {
    paraMotor();
    return;
  }
  movimentoDesdeMs = millis();
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

void imprimirEstadoHall() {
  Serial.print(F("Hall D7,D8,D11,D12: "));
  for (int i = 0; i < 4; i++) Serial.print(hallAtivo[i] ? '1' : '0');
  Serial.println();
}

void reportaEntradaPiso(int piso) {
  Serial.println(F("--------------------------------------"));
  if (sentido == SUBINDO) Serial.print(F(">>> SUBIDA — PISO "));
  else Serial.print(F(">>> DESCIDA — PISO "));
  Serial.println(piso);
  Serial.print(F("    Sensor D"));
  Serial.println(sensoresHall[piso - 1]);
  imprimirEstadoHall();
  Serial.println(F("--------------------------------------"));
}

void sincronizaBordasHall() {
  for (int i = 0; i < 4; i++) {
    hallConfirmadoAnt[i] = hallConfirmado[i];
  }
}

void processaBordasHall() {
  for (int i = 0; i < 4; i++) {
    if (hallConfirmado[i] && !hallConfirmadoAnt[i]) {
      int piso = i + 1;
      reportaEntradaPiso(piso);

      if (sentido == SUBINDO && piso == 4) {
        Serial.println(F("=== PISO 4 DETETADO — comeca descida ==="));
        arrancaSentido(DESCENDO);
      } else if (sentido == DESCENDO && piso == 1) {
        paraMotor();
        sentido = PARADO;
        Serial.println(F("=== PISO 1 DETETADO — teste completo ==="));
      }
    }
    hallConfirmadoAnt[i] = hallConfirmado[i];
  }
}

void setup() {
  Serial.begin(9600);
  delay(400);

  for (int i = 0; i < 4; i++) {
    pinMode(sensoresHall[i], INPUT_PULLUP);
    hallAtivoDesdeMs[i] = 0;
    hallConfirmadoAnt[i] = false;
  }

  pinMode(pinMotorIN1, OUTPUT);
  pinMode(pinMotorIN2, OUTPUT);
  pinMode(pinMotorIN3, OUTPUT);
  pinMode(pinMotorIN4, OUTPUT);
  paraMotor();

  Serial.println();
  Serial.println(F("=== TESTE HALL — SOBE / DETETA / DESCE ==="));
  Serial.println(F("Piso1=D7 Piso2=D8 Piso3=D11 Piso4=D12"));
  Serial.println(F("Ignora Hall ja activo no arranque (so reporta ao ENTRAR)"));
  Serial.println(F("--------------------------------------"));

  lerHall();
  delay(HALL_CONFIRMA_MS + 20);
  lerHall();
  sincronizaBordasHall();
  imprimirEstadoHall();

  for (int i = 0; i < 4; i++) {
    if (hallConfirmado[i]) {
      Serial.print(F("Arranque: Hall piso "));
      Serial.print(i + 1);
      Serial.println(F(" activo — ignorado, motor sobe..."));
      break;
    }
  }

  arrancaSentido(SUBINDO);
}

void loop() {
  motorService();
  lerHall();
  processaBordasHall();

  if (sentido == PARADO) return;

  if (millis() - movimentoDesdeMs >= TIMEOUT_MOV_MS) {
    paraMotor();
    sentido = PARADO;
    Serial.println(F("!! TIMEOUT — paragem de seguranca"));
    imprimirEstadoHall();
  }
}
