/*
  PAP — Teste Hall ×4 com saída para gráfico Python
  Pinos: D8, D11, D12, D13  |  LOW = íman detetado
  Serial 115200: linhas "p1,p2,p3,p4" (0–100) a cada 50 ms
*/

#include <Arduino.h>

const int PINS[4] = {8, 11, 12, 13};
const int SAMPLES_BURST = 12;
const float ALPHA_UP = 0.45f;
const float ALPHA_DOWN = 0.10f;
const unsigned long INTERVAL_MS = 50;

float level[4] = {0, 0, 0, 0};
unsigned long lastSend = 0;

float readInstant(int pin) {
  int lows = 0;
  for (int s = 0; s < SAMPLES_BURST; s++) {
    if (digitalRead(pin) == LOW) {
      lows++;
    }
    delayMicroseconds(150);
  }
  return (lows * 100.0f) / SAMPLES_BURST;
}

void smoothLevel(int i, float instant) {
  float alpha = (instant > level[i]) ? ALPHA_UP : ALPHA_DOWN;
  level[i] += alpha * (instant - level[i]);
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  for (int i = 0; i < 4; i++) {
    pinMode(PINS[i], INPUT_PULLUP);
  }

  Serial.println(F("# PAP Hall plot — aguardar linhas p1,p2,p3,p4"));
}

void loop() {
  for (int i = 0; i < 4; i++) {
    smoothLevel(i, readInstant(PINS[i]));
  }

  unsigned long now = millis();
  if (now - lastSend >= INTERVAL_MS) {
    lastSend = now;
    for (int i = 0; i < 4; i++) {
      if (i > 0) {
        Serial.print(',');
      }
      Serial.print((int)(level[i] + 0.5f));
    }
    Serial.println();
  }
}
