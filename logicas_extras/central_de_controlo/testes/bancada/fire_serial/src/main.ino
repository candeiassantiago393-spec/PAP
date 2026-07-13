/*
  Teste KY-026 — detecao de fogo (Serial 9600)
  Arduino Mega — ANALOGICO + DIGITAL

  Cablagem:
    (-) GND  -> GND Mega
    (+) VCC  -> 5V Mega
    AO       -> A0        (analógico)
    DO       -> pin 5     (digital — LOW = chama)

  Alarme se: DO deteta chama  OU  queda A0 >= limiar
*/

#include <Arduino.h>

#define PIN_AO A0
#define PIN_DO 5

#define READ_MS 400UL
#define AO_ALERT_DROP 60
#define DO_CHAMA_LOW 1

int baselineAo = 0;

void calibrarBaseline() {
  long sum = 0;
  for (int i = 0; i < 24; i++) {
    sum += analogRead(PIN_AO);
    delay(40);
  }
  baselineAo = (int)(sum / 24);
}

bool chamaDigital() {
  bool raw = digitalRead(PIN_DO);
#if DO_CHAMA_LOW
  return raw == LOW;
#else
  return raw == HIGH;
#endif
}

int quedaAnalogica(int raw) {
  int drop = baselineAo - raw;
  return drop > 0 ? drop : 0;
}

void setup() {
  Serial.begin(9600);
  delay(300);

  pinMode(PIN_DO, INPUT_PULLUP);

  Serial.println();
  Serial.println(F("========================================"));
  Serial.println(F("  TESTE KY-026 — FOGO"));
  Serial.println(F("========================================"));
  Serial.println(F("Pinos:"));
  Serial.println(F("  AO (analógico) -> A0"));
  Serial.println(F("  DO (digital)   -> pin 5"));
  Serial.println(F("  VCC -> 5V | GND -> GND"));
  Serial.println();
  Serial.println(F("Calibrar A0 sem chama..."));
  calibrarBaseline();
  Serial.print(F("Baseline A0 = "));
  Serial.println(baselineAo);
  Serial.println();
  Serial.println(F("Ajusta POT no modulo ate LED acender com isqueiro."));
  Serial.println(F("----------------------------------------"));
  Serial.println(F("A0 | queda | DO | analógico | digital | FOGO?"));
  Serial.println(F("----------------------------------------"));
}

void loop() {
  int raw = analogRead(PIN_AO);
  int queda = quedaAnalogica(raw);
  bool dig = chamaDigital();
  bool ana = (queda >= AO_ALERT_DROP);
  bool fogo = dig || ana;

  Serial.print(F("A0="));
  if (raw < 100) Serial.print(' ');
  if (raw < 10) Serial.print(' ');
  Serial.print(raw);

  Serial.print(F("  q="));
  if (queda < 10) Serial.print(' ');
  if (queda < 100) Serial.print(' ');
  Serial.print(queda);

  Serial.print(F("  DO="));
  Serial.print(digitalRead(PIN_DO) == LOW ? F("LOW ") : F("HIGH"));

  Serial.print(F("  ana="));
  Serial.print(ana ? F("SIM ") : F("NAO "));

  Serial.print(F(" dig="));
  Serial.print(dig ? F("SIM ") : F("NAO "));

  if (fogo) Serial.println(F(" >>> FOGO <<<"));
  else Serial.println(F(" ok"));

  delay(READ_MS);
}
