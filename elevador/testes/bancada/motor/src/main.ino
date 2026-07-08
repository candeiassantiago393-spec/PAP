/*
  Teste bancada — motor passo bipolar NEMA 17 via L298N
  Mesma pinagem e sequencia do firmware real (elevador/real/l298n_sh1106_hall).

  Ligações L298N <-> Mega:
    IN1 -> D9
    IN2 -> D33
    IN3 -> D31
    IN4 -> D32
    GND Mega <-> GND L298N (obrigatório)

  Alimentação motor (separada do USB):
    Fonte 12 V -> +VIN motor do L298N
    GND fonte <-> GND L298N <-> GND Mega

  Bobinas motor (4 fios) -> OUT1/OUT2 e OUT3/OUT4 do L298N
  Se rodar ao contrário: troca um par de fios de uma bobina.

  Serial 9600:
    u = sobe continuo    d = desce continuo    s = para (mantem posicao)
    l = liberta bobinas  + = mais rapido       - = mais lento
    n = N passos sobe    p = N passos desce   (ex: n200)
    a = demo automatica  (200 passos sobe/desce)
*/

#include <Arduino.h>

#define pinMotorIN1 9
#define pinMotorIN2 33
#define pinMotorIN3 31
#define pinMotorIN4 32

#define VELOCIDADE_INICIAL 80
#define PASSOS_DEMO 200

volatile int stepRate = 0;
unsigned long lastStepMicros = 0;
int motorDir = 0;
int faseMotor = 0;
int velocidade = VELOCIDADE_INICIAL;

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
  aplicaFaseMotor(faseMotor);
}

void arrancaMotor(int dir) {
  motorDir = dir;
  stepRate = velocidade;
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

void moverPassos(int dir, int passos) {
  if (passos <= 0) return;

  for (int i = 0; i < passos; i++) {
    if (dir > 0) {
      faseMotor = (faseMotor + 1) & 0x03;
    } else {
      faseMotor = (faseMotor + 3) & 0x03;
    }
    aplicaFaseMotor(faseMotor);

    unsigned long intervalo = 1000000UL / (unsigned long)velocidade;
    delayMicroseconds(intervalo);
  }

  paraMotor();
}

void imprimirAjuda() {
  Serial.println();
  Serial.println("=== TESTE MOTOR L298N ===");
  Serial.println("IN1=D9 IN2=D33 IN3=D31 IN4=D32");
  Serial.println("Comandos: u sobe | d desce | s para (segura)");
  Serial.println("          l liberta bobinas (sem torque)");
  Serial.println("          + rapido | - lento");
  Serial.println("          n200 = 200 passos sobe | p200 = desce");
  Serial.println("          a = demo auto (200 passos)");
  Serial.print("Velocidade: ");
  Serial.print(velocidade);
  Serial.println(" passos/s");
  Serial.println("-------------------------");
}

void processarComando(char c) {
  switch (c) {
    case 'u':
    case 'U':
      arrancaMotor(+1);
      Serial.println(">> SOBE continuo");
      break;
    case 'd':
    case 'D':
      arrancaMotor(-1);
      Serial.println(">> DESCE continuo");
      break;
    case 's':
    case 'S':
      paraMotor();
      Serial.println(">> PARADO (bobinas activas — segura posicao)");
      break;
    case 'l':
    case 'L':
      stepRate = 0;
      motorDir = 0;
      desligaBobinasMotor();
      Serial.println(">> LIBERTO (sem torque)");
      break;
    case '+':
      velocidade = min(velocidade + 10, 400);
      if (motorDir != 0) stepRate = velocidade;
      Serial.print("Velocidade: ");
      Serial.println(velocidade);
      break;
    case '-':
      velocidade = max(velocidade - 10, 20);
      if (motorDir != 0) stepRate = velocidade;
      Serial.print("Velocidade: ");
      Serial.println(velocidade);
      break;
    case 'a':
    case 'A':
      paraMotor();
      Serial.println(">> Demo: sobe...");
      moverPassos(+1, PASSOS_DEMO);
      delay(800);
      Serial.println(">> Demo: desce...");
      moverPassos(-1, PASSOS_DEMO);
      Serial.println(">> Demo fim");
      break;
    default:
      break;
  }
}

void setup() {
  pinMode(pinMotorIN1, OUTPUT);
  pinMode(pinMotorIN2, OUTPUT);
  pinMode(pinMotorIN3, OUTPUT);
  pinMode(pinMotorIN4, OUTPUT);
  desligaBobinasMotor();

  Serial.begin(9600);
  delay(400);
  imprimirAjuda();
}

void loop() {
  motorService();

  if (Serial.available()) {
    String linha = Serial.readStringUntil('\n');
    linha.trim();

    if (linha.length() == 0) return;

    char cmd = linha.charAt(0);

    if ((cmd == 'n' || cmd == 'N' || cmd == 'p' || cmd == 'P') && linha.length() > 1) {
      int passos = linha.substring(1).toInt();
      if (passos <= 0) passos = PASSOS_DEMO;
      paraMotor();
      if (cmd == 'n' || cmd == 'N') {
        Serial.print(">> ");
        Serial.print(passos);
        Serial.println(" passos SOBE");
        moverPassos(+1, passos);
      } else {
        Serial.print(">> ");
        Serial.print(passos);
        Serial.println(" passos DESCE");
        moverPassos(-1, passos);
      }
      return;
    }

    processarComando(cmd);
  }
}
