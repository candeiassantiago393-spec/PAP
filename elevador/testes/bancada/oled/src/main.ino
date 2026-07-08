/*
  Diagnostico OLED — teste rapido I2C (nao bloqueia)
  Mega: SDA=20, SCL=21 | TCA tipico 0x70 | OLED tipico 0x3C
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define OLED_W 128
#define OLED_H 64

Adafruit_SH1106G display(OLED_W, OLED_H, &Wire, -1);

int ping(uint8_t addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission();
}

void tcaSelect(uint8_t tcaAddr, uint8_t ch) {
  Wire.beginTransmission(tcaAddr);
  Wire.write(1 << ch);
  Wire.endTransmission();
}

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  delay(500);
  Serial.println();
  Serial.println("===== TESTE OLED RAPIDO =====");

  Wire.begin();
  Wire.setClock(100000);

  Serial.print("Ping TCA 0x70: ");
  Serial.println(ping(0x70) == 0 ? "OK" : "FALHOU");

  Serial.print("Ping OLED 0x3C directo: ");
  Serial.println(ping(0x3C) == 0 ? "OK" : "FALHOU");

  uint8_t tca = 0;
  if (ping(0x70) == 0) tca = 0x70;

  if (tca) {
    for (uint8_t ch = 0; ch < 4; ch++) {
      tcaSelect(tca, ch);
      Serial.print("Canal ");
      Serial.print(ch);
      Serial.print(" OLED 0x3C: ");
      Serial.println(ping(0x3C) == 0 ? "OK" : "FALHOU");
    }

    tcaSelect(tca, 0);
    if (ping(0x3C) == 0 && display.begin(0x3C, true)) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(8, 20);
      display.print("OLED OK");
      display.display();
      Serial.println("Display canal 0: texto enviado");
      digitalWrite(13, HIGH);
    } else {
      Serial.println("begin() falhou no canal 0");
    }
  } else {
    Serial.println();
    Serial.println("PROBLEMA: I2C nao responde.");
    Serial.println("Verifica NA ORDEM:");
    Serial.println(" 1) GND comum Mega + TCA + OLED");
    Serial.println(" 2) 5V ao TCA9548A e aos OLED");
    Serial.println(" 3) SDA Mega pin20 -> SDA do TCA");
    Serial.println(" 4) SCL Mega pin21 -> SCL do TCA");
    Serial.println(" 5) OLEDs nos canais SD0-SD3/SC0-SC3 do TCA");
    Serial.println(" 6) Cabo SDA/SCL nao invertido");
    Serial.println(" 7) Desliga motor/L298N e testa (ruido)");
  }

  Serial.println("=============================");
}

void loop() {
  delay(1000);
  digitalWrite(13, !digitalRead(13));
}
