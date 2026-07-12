/*
  Diagnostico Hall — foco D7/D8 (pisos 1 e 2) vs D11/D12 (pisos 3 e 4)

  Teste:
    1. Carrega isto e abre Serial 9600
    2. Aproxima iman ao sensor 1 -> D7 deve ir a LOW*
    3. Sensor 2 -> D8 LOW*
    4. Se LED acende mas D7/D8 ficam HIGH -> fio OUT nao chega ao Mega

  Teste extra: desliga OUT do sensor 1 do Mega e toca em GND
    -> se D7=LOW* o Mega esta OK, culpa e o cabo/modulo OUT
*/

#include <Arduino.h>

struct PinInfo {
  uint8_t pin;
  const char* label;
  const char* piso;
};

const PinInfo PINS[] = {
  {7, "D7", "PISO 1"},
  {8, "D8", "PISO 2"},
  {11, "D11", "PISO 3"},
  {12, "D12", "PISO 4"},
};
const int N = 4;

bool lastLow[N];
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(9600);
  delay(400);

  for (int i = 0; i < N; i++) {
    pinMode(PINS[i].pin, INPUT_PULLUP);
    lastLow[i] = (digitalRead(PINS[i].pin) == LOW);
  }

  Serial.println();
  Serial.println(F("=== DIAG HALL PISOS 1-4 ==="));
  Serial.println(F("Piso1=D7  Piso2=D8  Piso3=D11  Piso4=D12"));
  Serial.println(F("LOW* = iman / GND   |   HIGH = nada"));
  Serial.println(F("--- Aproxima iman a CADA sensor ---"));
  Serial.println();
}

void loop() {
  bool low[N];
  for (int i = 0; i < N; i++) {
    low[i] = (digitalRead(PINS[i].pin) == LOW);
    if (low[i] != lastLow[i]) {
      Serial.print(F(">> MUDANCA "));
      Serial.print(PINS[i].piso);
      Serial.print(F(" ("));
      Serial.print(PINS[i].label);
      Serial.print(F("): "));
      Serial.println(low[i] ? F("AGORA LOW (detetado)") : F("AGORA HIGH (soltou)"));
      lastLow[i] = low[i];
    }
  }

  unsigned long now = millis();
  if (now - lastPrint >= 800) {
    lastPrint = now;
    Serial.print(F("Estado: "));
    for (int i = 0; i < N; i++) {
      if (i > 0) Serial.print(F(" | "));
      Serial.print(PINS[i].label);
      Serial.print('=');
      Serial.print(low[i] ? F("LOW*") : F("HIGH"));
    }
    Serial.println();
  }

  delay(15);
}
