/*
  Teste PIR HC-SR501 — detecao de movimento (Serial 9600)
  Arduino Mega

  Cablagem (modulo mini — foto bancada):
    +  vermelho (esq)  -> 5V
    ?  branco (meio)   -> GND
    AL preto (dir)    -> pin 5   (HIGH = movimento)

  Aguarda ~30 s apos ligar — o modulo precisa de aquecer.
*/

#include <Arduino.h>

#define PIN_PIR 5
#define READ_MS 200UL
#define PIR_WARMUP_MS 30000UL

bool pirLast = LOW;
unsigned long detectCount = 0;

void setup() {
  Serial.begin(9600);
  delay(300);

  pinMode(PIN_PIR, INPUT);

  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("  TESTE PIR — MOVIMENTO"));
  Serial.println(F("========================================"));
  Serial.println(F("Pinos:"));
  Serial.println(F("  + vermelho -> 5V"));
  Serial.println(F("  branco     -> GND"));
  Serial.println(F("  AL preto   -> pin 5"));
  Serial.println();
  Serial.println(F("Aguarda 30 s (aquecimento do PIR)..."));

  unsigned long start = millis();
  while (millis() - start < PIR_WARMUP_MS) {
    if ((millis() - start) % 5000 < READ_MS) {
      Serial.print(F("  "));
      Serial.print((PIR_WARMUP_MS - (millis() - start)) / 1000);
      Serial.println(F(" s restantes"));
    }
    delay(READ_MS);
  }

  pirLast = digitalRead(PIN_PIR);
  Serial.println();
  Serial.println(F("Pronto. Passa a mao em frente ao PIR."));
  Serial.println(F("----------------------------------------"));
  Serial.println(F("estado | deteccoes | nota"));
  Serial.println(F("----------------------------------------"));
}

void loop() {
  bool pirNow = digitalRead(PIN_PIR);

  if (pirNow == HIGH && pirLast == LOW) {
    detectCount++;
    Serial.print(F("HIGH   | "));
    Serial.print(detectCount);
    Serial.println(F("         >>> MOVIMENTO <<<"));
  } else if (pirNow == LOW && pirLast == HIGH) {
    Serial.println(F("LOW    | -            sem movimento"));
  }

  pirLast = pirNow;
  delay(READ_MS);
}
