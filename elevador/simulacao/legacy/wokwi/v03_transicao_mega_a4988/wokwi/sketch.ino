#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// --- A4988 (Stepper) ---
#define pinSTEP 9
#define pinDIR  10

// velocidades agora são "passos por segundo" (não é PWM!)
#define velocidadeSobe   600   // ajusta: 200..1200
#define velocidadeDesce  350   // ajusta: 150..1000

// --- Entradas/Saídas (mantidas) ---
#define pinBotaoA 2
#define pinBotaoB 3
#define pinBotaoC 4
#define pinBotaoD 5

#define pinSensorA 8
#define pinSensorB 11
#define pinSensorC 12
#define pinSensorD 13

#define pinBuzzer 6

#define pinLedA A0
#define pinLedB A1
#define pinLedC A2
#define pinLedD A3

hd44780_I2Cexp lcd;

String ponteiro[2] = { "v", " " };
int ledAndar[4] = { pinLedA, pinLedB, pinLedC, pinLedD };

// Pedidos de andares
bool pedidos[4] = { false, false, false, false };

bool estadoSensorA, estadoSensorB, estadoSensorC, estadoSensorD;
bool estadoSensorAAnt, estadoSensorBAnt, estadoSensorCAnt, estadoSensorDAnt;

int acaoElevador = 0;      // 0=parado, 1=subir, 2=descer
int andar = -1;
int andarDestino = -1;

// --- Stepper runtime ---
volatile int stepRate = 0;                 // passos/seg
unsigned long lastStepMicros = 0;

// --- Buzzer (não-bloqueante) ---
bool beepAtivo = false;
unsigned long beepAteMs = 0;

// cadências para não matar o stepper com I2C lento
unsigned long lastLogicMs = 0;
unsigned long lastLcdMs   = 0;

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

void beepCurto() {
  tone(pinBuzzer, 800);
  beepAtivo = true;
  beepAteMs = millis() + 120;
}

void atualizaLcdPonteiro() {
  lcd.setCursor(1, 0);  lcd.print(ponteiro[digitalRead(pinSensorA)]);
  lcd.setCursor(5, 0);  lcd.print(ponteiro[digitalRead(pinSensorB)]);
  lcd.setCursor(9, 0);  lcd.print(ponteiro[digitalRead(pinSensorC)]);
  lcd.setCursor(13, 0); lcd.print(ponteiro[digitalRead(pinSensorD)]);
}

void setup() {
  pinMode(pinBotaoA, INPUT_PULLUP);
  pinMode(pinBotaoB, INPUT_PULLUP);
  pinMode(pinBotaoC, INPUT_PULLUP);
  pinMode(pinBotaoD, INPUT_PULLUP);

  pinMode(pinLedA, OUTPUT);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinLedC, OUTPUT);
  pinMode(pinLedD, OUTPUT);

  // sensores: evita flutuação; activo a 0 (LOW)
  pinMode(pinSensorA, INPUT_PULLUP);
  pinMode(pinSensorB, INPUT_PULLUP);
  pinMode(pinSensorC, INPUT_PULLUP);
  pinMode(pinSensorD, INPUT_PULLUP);

  pinMode(pinBuzzer, OUTPUT);

  pinMode(pinSTEP, OUTPUT);
  pinMode(pinDIR, OUTPUT);
  digitalWrite(pinSTEP, LOW);

  lcd.begin(20, 4);
  lcd.backlight();

  lcd.setCursor(0, 1);
  lcd.print("(1) (2) (3) (4)");

  identificaAndar();

  // inicializa "anteriores" para não gerar falso evento
  estadoSensorAAnt = estadoSensorA;
  estadoSensorBAnt = estadoSensorB;
  estadoSensorCAnt = estadoSensorC;
  estadoSensorDAnt = estadoSensorD;
}

void loop() {
  // serviços sempre a correr (motor + buzzer)
  stepperService();
  beepService();

  // lógica a cada 10ms (mantém stepper suave)
  if (millis() - lastLogicMs < 10) return;
  lastLogicMs = millis();

  // 1) Ler botões e registar pedidos
  if (!digitalRead(pinBotaoA)) pedidos[0] = true;
  if (!digitalRead(pinBotaoB)) pedidos[1] = true;
  if (!digitalRead(pinBotaoC)) pedidos[2] = true;
  if (!digitalRead(pinBotaoD)) pedidos[3] = true;

  // 2) Ler sensores (activo a LOW)
  estadoSensorA = !digitalRead(pinSensorA);
  estadoSensorB = !digitalRead(pinSensorB);
  estadoSensorC = !digitalRead(pinSensorC);
  estadoSensorD = !digitalRead(pinSensorD);

  // tocar buzzer em cada pedido atendido (sem delay)
  if (estadoSensorA && !estadoSensorAAnt) {
    andar = 1;
    if (pedidos[0]) beepCurto();
    pedidos[0] = false;
  }
  if (estadoSensorB && !estadoSensorBAnt) {
    andar = 2;
    if (pedidos[1]) beepCurto();
    pedidos[1] = false;
  }
  if (estadoSensorC && !estadoSensorCAnt) {
    andar = 3;
    if (pedidos[2]) beepCurto();
    pedidos[2] = false;
  }
  if (estadoSensorD && !estadoSensorDAnt) {
    andar = 4;
    if (pedidos[3]) beepCurto();
    pedidos[3] = false;
  }

  // 3) Escolher próximo destino
  int proxAcima = -1, proxAbaixo = -1;

  if (andar != -1) {
    for (int i = andar; i < 4; i++)
      if (pedidos[i]) { proxAcima = i + 1; break; }

    for (int i = andar - 2; i >= 0; i--)
      if (pedidos[i]) { proxAbaixo = i + 1; break; }
  }

  if (proxAcima != -1) andarDestino = proxAcima;
  else if (proxAbaixo != -1) andarDestino = proxAbaixo;
  else andarDestino = andar;

  // 4) Definir ação do elevador
  if (andar == andarDestino) acaoElevador = 0;
  else if (andar < andarDestino) acaoElevador = 1;
  else acaoElevador = 2;

  // 5) Motor (A4988)
  if (acaoElevador == 0) {
    stepRate = 0;                 // parar = sem pulsos
  }
  if (acaoElevador == 1) {
    digitalWrite(pinDIR, HIGH);   // subir
    stepRate = velocidadeSobe;
  }
  if (acaoElevador == 2) {
    digitalWrite(pinDIR, LOW);    // descer
    stepRate = velocidadeDesce;
  }

  // 6) LEDs dos pedidos
  for (int i = 0; i < 4; i++)
    digitalWrite(ledAndar[i], pedidos[i] ? HIGH : LOW);

  // 7) Ponteiro no LCD (actualiza só a cada 100ms para não travar o stepper)
  if (millis() - lastLcdMs >= 100) {
    lastLcdMs = millis();
    atualizaLcdPonteiro();
  }

  // Guardar estados
  estadoSensorAAnt = estadoSensorA;
  estadoSensorBAnt = estadoSensorB;
  estadoSensorCAnt = estadoSensorC;
  estadoSensorDAnt = estadoSensorD;
}

void identificaAndar() {
  // lê sensores
  estadoSensorA = !digitalRead(pinSensorA);
  estadoSensorB = !digitalRead(pinSensorB);
  estadoSensorC = !digitalRead(pinSensorC);
  estadoSensorD = !digitalRead(pinSensorD);

  andar = -1;
  if (estadoSensorA) andar = 1;
  if (estadoSensorB) andar = 2;
  if (estadoSensorC) andar = 3;
  if (estadoSensorD) andar = 4;

  // Se não estiver em nenhum andar → desce até encontrar um
  if (andar == -1) {
    digitalWrite(pinDIR, LOW);
    stepRate = velocidadeDesce;
  }

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

  // parar quando encontrou
  stepRate = 0;

  andarDestino = andar;

  for (int i = 0; i < 4; i++)
    pedidos[i] = false;
}
