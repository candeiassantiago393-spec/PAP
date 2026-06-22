#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define pinMotorA 9    //Motor DC
#define pinMotorB 10   //Motor DC
#define pinBotaoA 2    //Botão - Andar 1
#define pinBotaoB 3    //Botão - Andar 2
#define pinBotaoC 4    //Botão - Andar 3
#define pinBotaoD 5    //Botão - Andar 4
#define pinSensorA 8   //Sensor - Andar 1
#define pinSensorB 11  //Sensor - Andar 2
#define pinSensorC 12  //Sensor - Andar 3
#define pinSensorD 13  //Sensor - Andar 4
#define pinBuzzer 6    //Buzzer
#define pinLedA A0     //LED - Andar 1
#define pinLedB A1     //LED - Andar 2
#define pinLedC A2     //LED - Andar 3
#define pinLedD A3     //LED - Andar 4

#define velocidadeSobe 200  
#define velocidadeDesce 90   

LiquidCrystal_I2C lcd(0x27, 20, 4);

String ponteiro[2] = { "v", " " };
int ledAndar[4] = { pinLedA, pinLedB, pinLedC, pinLedD };

// Pedidos de andares
bool pedidos[4] = { false, false, false, false };

bool estadoSensorA, estadoSensorB, estadoSensorC, estadoSensorD;
bool estadoSensorAAnt, estadoSensorBAnt, estadoSensorCAnt, estadoSensorDAnt;

int acaoElevador = 0;      
int acaoElevadorAnt = 0;

int andar = -1;            
int andarDestino = -1;     

void setup() {
  pinMode(pinBotaoA, INPUT_PULLUP);
  pinMode(pinBotaoB, INPUT_PULLUP);
  pinMode(pinBotaoC, INPUT_PULLUP);
  pinMode(pinBotaoD, INPUT_PULLUP);

  pinMode(pinLedA, OUTPUT);
  pinMode(pinLedB, OUTPUT);
  pinMode(pinLedC, OUTPUT);
  pinMode(pinLedD, OUTPUT);

  pinMode(pinSensorA, INPUT);
  pinMode(pinSensorB, INPUT);
  pinMode(pinSensorC, INPUT);
  pinMode(pinSensorD, INPUT);

  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinMotorA, OUTPUT);
  pinMode(pinMotorB, OUTPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 1);
  lcd.print("(1) (2) (3) (4)");

  identificaAndar();
}

void loop() {

  // 1) Ler botões e registar pedidos
  if (!digitalRead(pinBotaoA)) pedidos[0] = true;
  if (!digitalRead(pinBotaoB)) pedidos[1] = true;
  if (!digitalRead(pinBotaoC)) pedidos[2] = true;
  if (!digitalRead(pinBotaoD)) pedidos[3] = true;

  // 2) Ler sensores
  estadoSensorA = !digitalRead(pinSensorA);
  estadoSensorB = !digitalRead(pinSensorB);
  estadoSensorC = !digitalRead(pinSensorC);
  estadoSensorD = !digitalRead(pinSensorD);

  // --- ATUALIZAÇÃO: tocar buzzer em *cada* pedido atendido ---
  if (estadoSensorA && !estadoSensorAAnt) {
    andar = 1;
    if (pedidos[0]) { tone(pinBuzzer, 800); delay(120); noTone(pinBuzzer); }
    pedidos[0] = false;
  }
  if (estadoSensorB && !estadoSensorBAnt) {
    andar = 2;
    if (pedidos[1]) { tone(pinBuzzer, 800); delay(120); noTone(pinBuzzer); }
    pedidos[1] = false;
  }
  if (estadoSensorC && !estadoSensorCAnt) {
    andar = 3;
    if (pedidos[2]) { tone(pinBuzzer, 800); delay(120); noTone(pinBuzzer); }
    pedidos[2] = false;
  }
  if (estadoSensorD && !estadoSensorDAnt) {
    andar = 4;
    if (pedidos[3]) { tone(pinBuzzer, 800); delay(120); noTone(pinBuzzer); }
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

  // 5) Motor
  if (acaoElevador == 0) {
    analogWrite(pinMotorA, 255);
    analogWrite(pinMotorB, 255);
  }
  if (acaoElevador == 1) {
    analogWrite(pinMotorA, 0);
    analogWrite(pinMotorB, velocidadeSobe);
  }
  if (acaoElevador == 2) {
    analogWrite(pinMotorA, velocidadeDesce);
    analogWrite(pinMotorB, 0);
  }

  // 6) LEDs dos pedidos
  for (int i = 0; i < 4; i++)
    digitalWrite(ledAndar[i], pedidos[i] ? HIGH : LOW);

  // 7) Ponteiro no LCD
  lcd.setCursor(1, 0);  lcd.print(ponteiro[digitalRead(pinSensorA)]);
  lcd.setCursor(5, 0);  lcd.print(ponteiro[digitalRead(pinSensorB)]);
  lcd.setCursor(9, 0);  lcd.print(ponteiro[digitalRead(pinSensorC)]);
  lcd.setCursor(13, 0); lcd.print(ponteiro[digitalRead(pinSensorD)]);

  // Guardar estados
  estadoSensorAAnt = estadoSensorA;
  estadoSensorBAnt = estadoSensorB;
  estadoSensorCAnt = estadoSensorC;
  estadoSensorDAnt = estadoSensorD;

  acaoElevadorAnt = acaoElevador;

  delay(20);
}

void identificaAndar() {

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
    analogWrite(pinMotorA, velocidadeDesce);
    analogWrite(pinMotorB, 0);
  }

  while (andar == -1) {
    estadoSensorA = !digitalRead(pinSensorA);
    estadoSensorB = !digitalRead(pinSensorB);
    estadoSensorC = !digitalRead(pinSensorC);
    estadoSensorD = !digitalRead(pinSensorD);

    if (estadoSensorA) andar = 1;
    if (estadoSensorB) andar = 2;
    if (estadoSensorC) andar = 3;
    if (estadoSensorD) andar = 4;
  }

  analogWrite(pinMotorA, 255);
  analogWrite(pinMotorB, 255);

  andarDestino = andar;

  for (int i = 0; i < 4; i++)
    pedidos[i] = false;
}

