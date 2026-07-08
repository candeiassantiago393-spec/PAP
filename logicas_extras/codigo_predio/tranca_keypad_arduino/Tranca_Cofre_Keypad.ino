#include <Servo.h>
#include <Keypad.h>

Servo Servo1_3;

#define ledvermelho 11
#define ledverde 12
#define buzzer 13

String input = "";
String codigoCorreto = "1904";

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

void setup() {
  Serial.begin(9600);

  Servo1_3.attach(9, 500, 2500);

  pinMode(ledvermelho, OUTPUT);
  pinMode(ledverde, OUTPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(ledvermelho, LOW);
  digitalWrite(ledverde, LOW);

  Servo1_3.write(0);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '#') {
      if (input == codigoCorreto) {
        Serial.println("ACESSO OKAY");

        digitalWrite(ledvermelho, LOW);
        digitalWrite(ledverde, HIGH);

        Servo1_3.write(90);
        tone(buzzer, 1000, 200);
      } else {
        Serial.println("ACESSO NEGADO");

        digitalWrite(ledvermelho, HIGH);
        digitalWrite(ledverde, LOW);

        tone(buzzer, 200, 500);
      }

      input = "";
    }

    else if (key == '*') {
      input = "";
      Serial.println("RESET");
    }

    else {
      input += key;
      Serial.print("Codigo atual: ");
      Serial.println(input);
    }
  }
}
