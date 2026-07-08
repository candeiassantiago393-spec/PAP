/*
  Teste bancada — botões + LEDs pedidos exteriores (Arduino Mega)
  Pinagem: botões D2-D5 | LEDs A0-A3
*/

#define pinButtonExt1 2
#define pinButtonExt2 3
#define pinButtonExt3 4
#define pinButtonExt4 5

#define pinLedExt1 A0
#define pinLedExt2 A1
#define pinLedExt3 A2
#define pinLedExt4 A3

bool requestFloor1 = false;
bool requestFloor2 = false;
bool requestFloor3 = false;
bool requestFloor4 = false;

bool prevButton1 = HIGH;
bool prevButton2 = HIGH;
bool prevButton3 = HIGH;
bool prevButton4 = HIGH;

void printStatus() {
  Serial.print("LEDs: P1=");
  Serial.print(requestFloor1 ? "ON" : "off");
  Serial.print(" P2=");
  Serial.print(requestFloor2 ? "ON" : "off");
  Serial.print(" P3=");
  Serial.print(requestFloor3 ? "ON" : "off");
  Serial.print(" P4=");
  Serial.println(requestFloor4 ? "ON" : "off");
}

void setup() {
  Serial.begin(9600);
  while (!Serial && millis() < 3000) {
  }

  pinMode(pinButtonExt1, INPUT_PULLUP);
  pinMode(pinButtonExt2, INPUT_PULLUP);
  pinMode(pinButtonExt3, INPUT_PULLUP);
  pinMode(pinButtonExt4, INPUT_PULLUP);

  pinMode(pinLedExt1, OUTPUT);
  pinMode(pinLedExt2, OUTPUT);
  pinMode(pinLedExt3, OUTPUT);
  pinMode(pinLedExt4, OUTPUT);

  digitalWrite(pinLedExt1, LOW);
  digitalWrite(pinLedExt2, LOW);
  digitalWrite(pinLedExt3, LOW);
  digitalWrite(pinLedExt4, LOW);

  Serial.println();
  Serial.println("=== TESTE BOTOES + LEDs EXTERIORES ===");
  Serial.println("Botoes: D2 Piso1 | D3 Piso2 | D4 Piso3 | D5 Piso4");
  Serial.println("LEDs:   A0 Piso1 | A1 Piso2 | A2 Piso3 | A3 Piso4");
  Serial.println("Carrega num botao -> pedido fica latched + LED ON");
  Serial.println("--------------------------------------");
  printStatus();
}

void loop() {
  bool btn1 = digitalRead(pinButtonExt1);
  bool btn2 = digitalRead(pinButtonExt2);
  bool btn3 = digitalRead(pinButtonExt3);
  bool btn4 = digitalRead(pinButtonExt4);

  if (btn1 == LOW && prevButton1 == HIGH) {
    requestFloor1 = true;
    Serial.println(">> Botao PISO 1 premido — pedido guardado");
    printStatus();
  }
  if (btn2 == LOW && prevButton2 == HIGH) {
    requestFloor2 = true;
    Serial.println(">> Botao PISO 2 premido — pedido guardado");
    printStatus();
  }
  if (btn3 == LOW && prevButton3 == HIGH) {
    requestFloor3 = true;
    Serial.println(">> Botao PISO 3 premido — pedido guardado");
    printStatus();
  }
  if (btn4 == LOW && prevButton4 == HIGH) {
    requestFloor4 = true;
    Serial.println(">> Botao PISO 4 premido — pedido guardado");
    printStatus();
  }

  prevButton1 = btn1;
  prevButton2 = btn2;
  prevButton3 = btn3;
  prevButton4 = btn4;

  digitalWrite(pinLedExt1, requestFloor1 ? HIGH : LOW);
  digitalWrite(pinLedExt2, requestFloor2 ? HIGH : LOW);
  digitalWrite(pinLedExt3, requestFloor3 ? HIGH : LOW);
  digitalWrite(pinLedExt4, requestFloor4 ? HIGH : LOW);

  delay(20);
}
