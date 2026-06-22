/*
  =============================================================================
  ELEVADOR 4 PISOS — TEST BENCH DOS DISPLAYS OLED (COMPATÍVEL COM WOKWI)
  =============================================================================
  Objetivo: Testar os 4 displays OLED de 1.3" (SH1106) ligados em paralelo.
  Endereço I2C: 0x3C
  
  Conexões Arduino Mega no Wokwi:
  - SDA = Pino 20 
  - SCL = Pino 21
  - VCC = 5V
  - GND = GND
*/

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h> // Biblioteca universal para displays, 100% suportada no Wokwi

// Configuração para o Display SH1106 de 128x64 por I2C (comunicação por Hardware [HW])
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);

void setup() {
  // Inicializa a comunicação Série a 9600 bps
  Serial.begin(9600);
  while (!Serial) {
    ; // Aguarda a abertura da porta série
  }
  
  Serial.println("=========================================");
  Serial.println("INICIANDO TEST BENCH - ELEVADOR DISPLAYS ");
  Serial.println("=========================================");

  // Inicializa o barramento I2C
  Wire.begin();

  // Teste de Hardware via varrimento I2C no endereço do display (0x3C)
  Wire.beginTransmission(0x3C);
  byte erroI2C = Wire.endTransmission();

  // Validação do estado das ligações físicas
  if (erroI2C == 0) {
    Serial.println("STATUS DO HARDWARE:");
    Serial.println("-> sda ok");
    Serial.println("-> scl ok");
    Serial.println("-> energizado");
    Serial.println("\nResultado: Comunicao I2C estabelecida com sucesso!");
  } 
  else {
    Serial.println("STATUS DO HARDWARE:");
    Serial.println("-> sda FALHOU (Sem resposta)");
    Serial.println("-> scl FALHOU (Sem resposta)");
    Serial.println("-> NAO energizado ou sem ligacao correta");
    Serial.println("\nResultado: ERRO! Verifique as ligacoes de 5V, GND, SDA e SCL.");
  }

  // Inicializa o ecrã OLED físico/simulado
  u8x8.begin();
  u8x8.setPowerSave(0); // Ativa o display (desliga o modo de poupança de energia)
  
  // Define a fonte de texto (recomenda-se uma fonte simples e legível de tamanho 8x16)
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_r);
  
  // Escreve a mensagem diretamente no ecrã (Coluna 2, Linha 3 para ficar centrado)
  u8x8.drawString(2, 3, "test bench");
  
  Serial.println("=========================================");
  Serial.println("Texto enviado para os displays.");
}

void loop() {
  // Mantém o estado estático do teste
}
