/*
  Código Prédio — teclado 096-4642 + servo
  Arduino Uno | Teclado 3×4 | Servo D9 | PIN 1904 + #

  Sem buzzer, sem LEDs — só validação de código e abertura da tranca.
*/

#include <Servo.h>
#include <Keypad.h>

Servo tranca;

const char *CODIGO_CORRETO = "1904";

const uint8_t ROWS = 4;
const uint8_t COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

uint8_t rowPins[ROWS] = {5, 4, 3, 2};
uint8_t colPins[COLS] = {8, 7, 6};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String input = "";

void abrirTranca() {
  for (int pos = 0; pos <= 90; pos++) {
    tranca.write(pos);
    delay(15);
  }
  delay(8000);
  for (int pos = 90; pos >= 0; pos--) {
    tranca.write(pos);
    delay(15);
  }
}

void setup() {
  Serial.begin(9600);
  tranca.attach(9, 500, 2500);
  tranca.write(0);
  Serial.println(F("Codigo predio — teclado 096-4642"));
}

void loop() {
  char key = keypad.getKey();

  if (!key) return;

  Serial.println(key);

  if (key == '#') {
    if (input == CODIGO_CORRETO) {
      Serial.println(F("ACESSO OK"));
      abrirTranca();
    } else {
      Serial.println(F("ACESSO NEGADO"));
    }
    input = "";
  } else if (key == '*') {
    input = "";
    Serial.println(F("RESET"));
  } else {
    input += key;
    Serial.print(F("Codigo: "));
    Serial.println(input);
  }
}
