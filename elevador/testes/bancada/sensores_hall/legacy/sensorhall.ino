/*
  =============================================================================
  ELEVADOR 4 PISOS — TEST BENCH APENAS PARA SENSORES HALL (WOKWI)
  =============================================================================
  Objetivo: Testar os 4 sensores de piso usando os botões/interruptores.
  Lógica: LOW = Magnetizado (Íman detetado) / HIGH = Não magnetizado
*/

#include <Arduino.h>

// Pinos dos sensores conforme o teu projeto
const int pinosSensores[4] = {8, 11, 12, 13};

unsigned long tempoAnterior = 0;
const unsigned long intervaloPrint = 500; // Atualiza o Serial Monitor a cada 500ms

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("=========================================");
  Serial.println("   TEST BENCH INTERATIVO: SENSORES HALL  ");
  Serial.println("=========================================");

  // Ativa o Pull-up interno para os botões funcionarem diretamente ao GND
  for (int i = 0; i < 4; i++) {
    pinMode(pinosSensores[i], INPUT_PULLUP);
  }
}

void loop() {
  bool estadoAtual[4];

  // Lê o estado de cada pino (LOW significa botão ativo/magnetizado)
  for (int i = 0; i < 4; i++) {
    estadoAtual[i] = (digitalRead(pinosSensores[i]) == LOW);
  }

  // Envia os dados para a consola a cada 500ms
  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervaloPrint) {
    tempoAnterior = tempoAtual;

    Serial.println("\n--- ESTADO DOS SENSORES IN REAL-TIME ---");
    for (int i = 0; i < 4; i++) {
      Serial.print("Piso ");
      Serial.print(i + 1);
      Serial.print(": ");
      
      if (estadoAtual[i]) {
        Serial.println("MAGNETIZADO");
      } else {
        Serial.println("nao magnetizado");
      }
    }
    Serial.println("----------------------------------------");
  }

  delay(20); // Pequena pausa estável
}
