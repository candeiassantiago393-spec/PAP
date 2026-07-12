/*
  Teste botão reset — módulo kit 3 pinos (VCC, GND, SIG)
  Mega: SIG -> D10

  Módulo típico kit:
    Soltar  = HIGH
    Premir  = LOW

  Serial 9600
  LED integrado (D13) aceso quando premido
*/

#include <Arduino.h>

#define PIN_RESET 10
#define LED_PIN 13

#define DEBOUNCE_MS 50UL

unsigned long lastPrint = 0;
int lastEstavel = -1;

bool rawAnt = HIGH;
bool estavel = HIGH;
unsigned long ultMudancaMs = 0;

void setup() {
  pinMode(PIN_RESET, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  delay(500);

  rawAnt = digitalRead(PIN_RESET);
  estavel = rawAnt;
  ultMudancaMs = millis();
  lastEstavel = estavel;

  Serial.println();
  Serial.println(F("=== TESTE BOTAO RESET D10 ==="));
  Serial.println(F("Modulo: VCC->5V  GND->GND  SIG->D10"));
  Serial.println(F("Premir = LOW | Soltar = HIGH"));
  Serial.println(F("LED D13 aceso = premido"));
  Serial.println(F("--------------------------------------"));
  Serial.print(F("Estado inicial: "));
  Serial.println(estavel == LOW ? F("PREMIDO") : F("SOLTO"));
}

void loop() {
  unsigned long agora = millis();
  bool raw = digitalRead(PIN_RESET);

  if (raw != rawAnt) {
    rawAnt = raw;
    ultMudancaMs = agora;
  }

  if ((agora - ultMudancaMs) >= DEBOUNCE_MS && raw != estavel) {
    estavel = raw;

    Serial.println();
    if (estavel == LOW) {
      Serial.println(F(">> PREMIDO (LOW) — reset activo"));
    } else {
      Serial.println(F(">> SOLTO (HIGH) — reset inactivo"));
    }
  }

  digitalWrite(LED_PIN, estavel == LOW ? HIGH : LOW);

  if (estavel != lastEstavel) {
    lastEstavel = estavel;
  }

  if (agora - lastPrint >= 400) {
    lastPrint = agora;
    Serial.print(F("D10="));
    Serial.print(estavel == LOW ? F("LOW") : F("HIGH"));
    Serial.print(F(" | Raw="));
    Serial.println(raw == LOW ? F("LOW") : F("HIGH"));
  }
}
