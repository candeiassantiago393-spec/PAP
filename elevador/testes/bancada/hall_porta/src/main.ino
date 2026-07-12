/*
  Teste permissiva porta — D22 (chave deslizante na maquete)
  Serial 9600

  LOW  = porta FECHADA  (permissiva valida — motor pode mover)
  HIGH = porta ABERTA   (movimento bloqueado)

  Cablagem chave:
    um terminal -> GND   outro -> D22
*/

#include <Arduino.h>

#define PIN_PORTA 22
#define LED_PIN 13

#define DEBOUNCE_MS 50UL

unsigned long lastPrint = 0;
bool rawAnt = HIGH;
bool estavel = HIGH;
unsigned long ultMudancaMs = 0;
int lastEstado = -1;  // 0=aberta 1=fechada

void setup() {
  pinMode(PIN_PORTA, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  delay(500);

  rawAnt = digitalRead(PIN_PORTA);
  estavel = rawAnt;
  ultMudancaMs = millis();

  Serial.println();
  Serial.println(F("=== TESTE HALL PORTA D22 ==="));
  Serial.println(F("LOW  = PORTA FECHADA"));
  Serial.println(F("HIGH = PORTA ABERTA"));
  Serial.println(F("LED D13 aceso = porta FECHADA"));
  Serial.println(F("--------------------------------------"));

  if (estavel == LOW) {
    Serial.println(F("Estado inicial: PORTA FECHADA"));
    lastEstado = 1;
  } else {
    Serial.println(F("Estado inicial: PORTA ABERTA"));
    lastEstado = 0;
  }
}

void loop() {
  unsigned long agora = millis();
  bool raw = digitalRead(PIN_PORTA);

  if (raw != rawAnt) {
    rawAnt = raw;
    ultMudancaMs = agora;
  }

  if ((agora - ultMudancaMs) >= DEBOUNCE_MS) {
    estavel = raw;
  }

  bool fechada = (estavel == LOW);
  digitalWrite(LED_PIN, fechada ? HIGH : LOW);

  int estado = fechada ? 1 : 0;
  if (estado != lastEstado) {
    lastEstado = estado;
    Serial.println();
    if (fechada) {
      Serial.println(F(">>> PORTA FECHADA <<<"));
      Serial.println(F("    (D22 = LOW — iman detectado)"));
    } else {
      Serial.println(F(">>> PORTA ABERTA <<<"));
      Serial.println(F("    (D22 = HIGH — sem iman)"));
    }
  }

  if (agora - lastPrint >= 500) {
    lastPrint = agora;
    Serial.print(F("D22="));
    Serial.print(estavel == LOW ? F("LOW") : F("HIGH"));
    Serial.print(F(" | "));
    Serial.println(fechada ? F("PORTA FECHADA") : F("PORTA ABERTA"));
  }
}
