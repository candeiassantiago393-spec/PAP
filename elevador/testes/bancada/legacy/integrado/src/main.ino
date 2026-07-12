/*
  Teste bancada integrado — Hall x4 + 4 OLED SH1106 + botões/LEDs exteriores
  Sem motor L298N, sem porta D22, sem botões interiores.

  Pinagem Mega:
    Hall pisos: D7, D8, D11, D12  (LOW = íman detetado)
    Botões ext: D2, D3, D4, D5
    LEDs ext:   A0, A1, A2, A3
    I2C:        SDA=20, SCL=21  (TCA9548A 0x70, OLED 0x3C nos canais 0-3)

  Serial 9600 — mensagens legíveis + estado a cada ~500 ms.
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define OLED_W 128
#define OLED_H 64
#define OLED_ADDR 0x3C
#define TCA_ADDR 0x70

#define pinBotaoExt1 2
#define pinBotaoExt2 3
#define pinBotaoExt3 4
#define pinBotaoExt4 5

#define pinLedExt1 A0
#define pinLedExt2 A1
#define pinLedExt3 A2
#define pinLedExt4 A3

#define pinSensor1 7
#define pinSensor2 8
#define pinSensor3 11
#define pinSensor4 12

Adafruit_SH1106G oled1(OLED_W, OLED_H, &Wire, -1);
Adafruit_SH1106G oled2(OLED_W, OLED_H, &Wire, -1);
Adafruit_SH1106G oled3(OLED_W, OLED_H, &Wire, -1);
Adafruit_SH1106G oled4(OLED_W, OLED_H, &Wire, -1);

Adafruit_SH1106G *const oleds[4] = {&oled1, &oled2, &oled3, &oled4};

const int botoesExt[4] = {pinBotaoExt1, pinBotaoExt2, pinBotaoExt3, pinBotaoExt4};
const int ledsExt[4] = {pinLedExt1, pinLedExt2, pinLedExt3, pinLedExt4};
const int sensoresHall[4] = {pinSensor1, pinSensor2, pinSensor3, pinSensor4};

bool pedidos[4] = {false, false, false, false};
bool prevBtn[4] = {HIGH, HIGH, HIGH, HIGH};
bool hallAtivo[4] = {false, false, false, false};

int andar = -1;
bool oledOk = false;
uint8_t oledCanal = 0;
unsigned long lastSerial = 0;
unsigned long lastOled = 0;

int ping(uint8_t addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission();
}

void tcaSelect(uint8_t channel) {
  if (channel > 7) return;
  Wire.beginTransmission(TCA_ADDR);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

bool initOleds() {
  if (ping(TCA_ADDR) != 0) {
    Serial.println("I2C: TCA 0x70 nao responde");
    return false;
  }

  for (uint8_t ch = 0; ch < 4; ch++) {
    tcaSelect(ch);
    if (ping(OLED_ADDR) != 0) {
      Serial.print("I2C: OLED canal ");
      Serial.print(ch);
      Serial.println(" nao responde");
      return false;
    }
    if (!oleds[ch]->begin(OLED_ADDR, true)) {
      Serial.print("I2C: begin() falhou canal ");
      Serial.println(ch);
      return false;
    }
    oleds[ch]->clearDisplay();
    oleds[ch]->display();
  }

  Serial.println("I2C: 4 OLEDs OK");
  return true;
}

void lerHall() {
  andar = -1;
  for (int i = 0; i < 4; i++) {
    hallAtivo[i] = (digitalRead(sensoresHall[i]) == LOW);
    if (hallAtivo[i]) {
      andar = i + 1;
    }
  }
}

void lerBotoes() {
  for (int i = 0; i < 4; i++) {
    bool btn = digitalRead(botoesExt[i]);
    if (btn == LOW && prevBtn[i] == HIGH) {
      pedidos[i] = true;
      Serial.print(">> Pedido PISO ");
      Serial.println(i + 1);
    }
    prevBtn[i] = btn;
  }
}

void atualizarLeds() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledsExt[i], pedidos[i] ? HIGH : LOW);
  }
}

void desenharOled(int idx) {
  Adafruit_SH1106G &d = *oleds[idx];
  int piso = idx + 1;

  d.clearDisplay();
  d.setTextColor(SH110X_WHITE);

  d.setTextSize(1);
  d.setCursor(0, 0);
  d.print("TESTE BANCADA");
  d.setCursor(90, 0);
  d.print("OLED");
  d.print(piso);

  d.setTextSize(2);
  d.setCursor(0, 14);
  d.print("PISO ");
  d.println(piso);

  d.setTextSize(1);
  d.setCursor(0, 36);
  d.print("Hall: ");
  d.println(hallAtivo[idx] ? "ATIVO" : "----");

  d.setCursor(0, 48);
  d.print("Ped: ");
  d.print(pedidos[idx] ? "SIM" : "nao");
  d.print("  LED:");
  d.println(pedidos[idx] ? "ON" : "off");

  d.setCursor(0, 58);
  d.print("Atual: ");
  if (andar >= 1 && andar <= 4) d.print(andar);
  else d.print("?");

  d.display();
}

void atualizarUmOled() {
  tcaSelect(oledCanal);
  desenharOled(oledCanal);
  oledCanal = (oledCanal + 1) % 4;
}

void imprimirSerial() {
  Serial.print("Andar=");
  if (andar >= 1) Serial.print(andar);
  else Serial.print("?");

  Serial.print(" | Hall:");
  for (int i = 0; i < 4; i++) {
    Serial.print(hallAtivo[i] ? '1' : '0');
  }

  Serial.print(" | Ped:");
  for (int i = 0; i < 4; i++) {
    Serial.print(pedidos[i] ? '1' : '0');
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  delay(400);

  for (int i = 0; i < 4; i++) {
    pinMode(botoesExt[i], INPUT_PULLUP);
    pinMode(ledsExt[i], OUTPUT);
    digitalWrite(ledsExt[i], LOW);
    pinMode(sensoresHall[i], INPUT_PULLUP);
  }

  Serial.println();
  Serial.println("=== TESTE INTEGRADO BANCADA ===");
  Serial.println("Hall D7,D8,D11,D12 | Botoes D2-D5 | LEDs A0-A3");
  Serial.println("OLEDs via TCA 0x70 canais 0-3 | Sem motor/porta");
  Serial.println("--------------------------------------");

  Wire.begin();
  Wire.setClock(100000);
  Wire.setWireTimeout(5000);

  oledOk = initOleds();
  if (!oledOk) {
    Serial.println("OLEDs desativados — Hall + botoes continuam");
  }

  lerHall();
  imprimirSerial();
}

void loop() {
  lerHall();
  lerBotoes();
  atualizarLeds();

  unsigned long now = millis();

  if (oledOk && now - lastOled >= 120) {
    lastOled = now;
    atualizarUmOled();
  }

  if (now - lastSerial >= 500) {
    lastSerial = now;
    imprimirSerial();
  }

  delay(15);
}
