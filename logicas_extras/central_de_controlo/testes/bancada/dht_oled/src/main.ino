/*
  Teste bancada — KY-015 (DHT11) + OLED SSD1306
  Mega: SDA=20, SCL=21 | KY-015 S pin 2 | BTN NEXT pin 3 | BTN PREV pin 4

  Ecrãs (rotação automática ou botões):
    0 HOME       — candeias.dev studio
    1 AMBIENTE   — temperatura + humidade
    2 GRAF TEMP  — gráfico temperatura
    3 GRAF HUM   — gráfico humidade
    4 INSTAGRAM  — QR code
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define OLED_RESET -1
#define PIN_DHT 2
#define PIN_BTN_NEXT 3
#define PIN_BTN_PREV 4
#define DHTTYPE DHT11

#define READ_INTERVAL_MS 2000
#define SCREEN_INTERVAL_MS 5000
#define DEBOUNCE_MS 35

#define SPARK_LEN 48
#define SPARK_X 4
#define SPARK_Y 14
#define SPARK_W 120
#define SPARK_H 42

#define QR_W 31
#define QR_H 31

// QR 31x31 — https://www.instagram.com/candeias.dev
const uint8_t PROGMEM instagramQr[] = {
  0x00, 0x00, 0x00, 0x00, 0x7F, 0x7C, 0x5D, 0xFC, 0x41, 0x5C, 0x05, 0x04,
  0x5D, 0x07, 0x15, 0x74, 0x5D, 0x6B, 0xA9, 0x74, 0x5D, 0x38, 0xCD, 0x74,
  0x41, 0x2D, 0x7D, 0x04, 0x7F, 0x55, 0x55, 0xFC, 0x00, 0x71, 0xF0, 0x00,
  0x5B, 0xBE, 0xF1, 0x2C, 0x6E, 0x1E, 0x3F, 0xC4, 0x17, 0x8A, 0x71, 0xD8,
  0x70, 0x6C, 0xDB, 0xC4, 0x69, 0xB9, 0xF0, 0x30, 0x14, 0xA1, 0xC9, 0x9C,
  0x7B, 0x12, 0x3A, 0xDC, 0x02, 0x09, 0x8B, 0xC8, 0x1D, 0x95, 0x12, 0x68,
  0x26, 0x75, 0x86, 0xB8, 0x45, 0xAF, 0xF6, 0x90, 0x16, 0xFB, 0x3B, 0x10,
  0x3D, 0x6F, 0xFF, 0xF0, 0x00, 0x44, 0x54, 0x7C, 0x7F, 0x75, 0x6D, 0x68,
  0x41, 0x57, 0x14, 0x60, 0x5D, 0x34, 0xA7, 0xD0, 0x5D, 0x60, 0xCE, 0x64,
  0x5D, 0x55, 0x7A, 0x94, 0x41, 0x0D, 0xD4, 0xA8, 0x7F, 0x5C, 0x1C, 0x28,
  0x00, 0x00, 0x00, 0x00,
};

enum ScreenId {
  SCREEN_HOME = 0,
  SCREEN_ENV = 1,
  SCREEN_GRAPH_TEMP = 2,
  SCREEN_GRAPH_HUM = 3,
  SCREEN_INSTAGRAM = 4,
  SCREEN_COUNT = 5
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dhtSensor(PIN_DHT, DHTTYPE);

ScreenId currentScreen = SCREEN_HOME;
unsigned long lastScreenChangeMs = 0;
unsigned long lastReadMs = 0;

float envTemp = NAN;
float envHum = NAN;

float tempHistory[SPARK_LEN];
float humHistory[SPARK_LEN];
uint8_t historyIdx = 0;
uint8_t historyCount = 0;

bool btnNextLastRaw = HIGH;
bool btnNextStable = HIGH;
unsigned long btnNextLastChangeMs = 0;

bool btnPrevLastRaw = HIGH;
bool btnPrevStable = HIGH;
unsigned long btnPrevLastChangeMs = 0;

const char *screenNames[] = {
  "HOME", "AMBIENTE", "GRAF TEMP", "GRAF HUM", "INSTAGRAM"
};

bool readDebouncedButtonPressed(int pin, bool &lastRaw, bool &stable,
                                unsigned long &lastChangeMs) {
  bool raw = digitalRead(pin);
  unsigned long now = millis();

  if (raw != lastRaw) {
    lastRaw = raw;
    lastChangeMs = now;
  }

  if ((now - lastChangeMs) > DEBOUNCE_MS && raw != stable) {
    stable = raw;
    if (stable == LOW) return true;
  }
  return false;
}

void floatToStr(char *out, size_t outLen, float value, uint8_t decimals) {
  if (outLen == 0) return;
  char tmp[16];
  dtostrf(value, 0, decimals, tmp);
  const char *p = tmp;
  while (*p == ' ') p++;
  strncpy(out, p, outLen - 1);
  out[outLen - 1] = '\0';
}

void logScreenChange() {
  Serial.print(F("Ecra: "));
  Serial.println(screenNames[currentScreen]);
}

void drawHeader(const char *title) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 0);
  display.print(title);
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
}

void drawTextCenteredAt(int centerX, int y, const char *text) {
  display.setTextSize(1);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  display.setCursor(centerX - (w / 2), y);
  display.print(text);
}

void drawTextRight(int y, const char *text) {
  display.setTextSize(1);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  display.setCursor(124 - w, y);
  display.print(text);
}

void drawHomeScreen() {
  drawHeader("HOME");
  display.setTextSize(1);
  display.setCursor(22, 22);
  display.print("candeias.dev");
  display.setCursor(46, 38);
  display.print("studio");
  display.setCursor(2, 54);
  display.print("N:");
  display.print(digitalRead(PIN_BTN_NEXT) == LOW ? '0' : '1');
  display.print(" P:");
  display.print(digitalRead(PIN_BTN_PREV) == LOW ? '0' : '1');
  display.print(" (0=premido)");
}

void drawEnvScreen() {
  char line[32];
  drawHeader("AMBIENTE");
  display.setTextSize(1);

  if (isnan(envTemp) || isnan(envHum)) {
    display.setCursor(4, 24);
    display.print("A ler KY-015...");
    display.setCursor(4, 38);
    display.print("Pin 2 (modulo S)");
    return;
  }

  char tStr[8];
  char hStr[8];
  floatToStr(tStr, sizeof(tStr), envTemp, 1);
  floatToStr(hStr, sizeof(hStr), envHum, 1);
  snprintf(line, sizeof(line), "Temperatura: %s C", tStr);
  display.setCursor(4, 20);
  display.print(line);

  snprintf(line, sizeof(line), "Humidade:    %s %%", hStr);
  display.setCursor(4, 34);
  display.print(line);
}

void pushSample(float temp, float hum) {
  if (!isnan(temp)) {
    tempHistory[historyIdx] = temp;
  }
  if (!isnan(hum)) {
    humHistory[historyIdx] = hum;
  }
  historyIdx = (historyIdx + 1) % SPARK_LEN;
  if (historyCount < SPARK_LEN) historyCount++;
}

float historyAt(const float *buf, uint8_t age) {
  if (historyCount < SPARK_LEN) {
    return buf[age];
  }
  return buf[(historyIdx + age) % SPARK_LEN];
}

void drawSparkline(const char *title, const float *buf, float current,
                   const char *unit) {
  char line[24];
  const int gx = SPARK_X;
  const int gy = SPARK_Y;
  const int innerW = SPARK_W - 2;
  const int innerH = SPARK_H - 2;

  drawHeader(title);
  display.drawRect(gx, gy, SPARK_W, SPARK_H, SSD1306_WHITE);

  if (historyCount < 2) {
    display.setCursor(gx + 18, gy + 16);
    display.print("A recolher...");
    if (!isnan(current)) {
      char vStr[8];
      floatToStr(vStr, sizeof(vStr), current, 1);
      snprintf(line, sizeof(line), "Atual: %s %s", vStr, unit);
      display.setCursor(gx + 18, gy + 28);
      display.print(line);
    }
    return;
  }

  float minV = historyAt(buf, 0);
  float maxV = historyAt(buf, 0);
  for (uint8_t i = 1; i < historyCount; i++) {
    float v = historyAt(buf, i);
    if (v < minV) minV = v;
    if (v > maxV) maxV = v;
  }
  if ((maxV - minV) < 1.0f) {
    minV -= 0.5f;
    maxV += 0.5f;
  }

  const int plotX = gx + 1;
  const int plotY = gy + 1;
  int prevPx = -1;
  int prevPy = -1;

  for (uint8_t i = 0; i < historyCount; i++) {
    float v = historyAt(buf, i);
    int px = plotX;
    if (historyCount > 1) {
      px = plotX + (i * innerW) / (historyCount - 1);
    }
    int py = plotY + innerH - 1 - (int)((v - minV) * innerH / (maxV - minV));
    py = constrain(py, plotY, plotY + innerH - 1);

    if (prevPx >= 0) {
      display.drawLine(prevPx, prevPy, px, py, SSD1306_WHITE);
    } else {
      display.drawPixel(px, py, SSD1306_WHITE);
    }
    prevPx = px;
    prevPy = py;
  }

  char minStr[8];
  char maxStr[8];
  floatToStr(minStr, sizeof(minStr), minV, 0);
  floatToStr(maxStr, sizeof(maxStr), maxV, 0);
  snprintf(line, sizeof(line), "Min:%s Max:%s", minStr, maxStr);
  display.setCursor(gx + 2, gy + SPARK_H + 2);
  display.print(line);

  if (!isnan(current)) {
    char vStr[8];
    floatToStr(vStr, sizeof(vStr), current, 1);
    snprintf(line, sizeof(line), "%s %s", vStr, unit);
    drawTextRight(gy + SPARK_H + 2, line);
  }
}

void drawInstagramScreen() {
  const int qrX = (SCREEN_WIDTH - QR_W) / 2;
  const int qrY = 6;
  display.drawBitmap(qrX, qrY, instagramQr, QR_W, QR_H, SSD1306_WHITE);
  drawTextCenteredAt(64, 44, "@candeias.dev");
  drawTextCenteredAt(64, 56, "Instagram");
}

void renderScreen() {
  display.clearDisplay();
  switch (currentScreen) {
    case SCREEN_HOME:
      drawHomeScreen();
      break;
    case SCREEN_ENV:
      drawEnvScreen();
      break;
    case SCREEN_GRAPH_TEMP:
      drawSparkline("GRAF TEMP", tempHistory, envTemp, "C");
      break;
    case SCREEN_GRAPH_HUM:
      drawSparkline("GRAF HUM", humHistory, envHum, "%");
      break;
    case SCREEN_INSTAGRAM:
      drawInstagramScreen();
      break;
    default:
      break;
  }
  display.display();
}

void diagnoseDhtPin() {
  pinMode(PIN_DHT, INPUT_PULLUP);
  delay(10);
  int state = digitalRead(PIN_DHT);
  Serial.print(F("Diag pin "));
  Serial.print(PIN_DHT);
  Serial.print(F(" (pull-up interno): "));
  Serial.println(state == HIGH ? F("HIGH — linha data OK em repouso")
                               : F("LOW — curto a GND ou ligacao errada"));
}

void diagnoseBtnPin(int pin, const __FlashStringHelper *label) {
  pinMode(pin, INPUT_PULLUP);
  delay(5);
  int state = digitalRead(pin);
  Serial.print(label);
  Serial.print(F(" pin "));
  Serial.print(pin);
  Serial.print(F(": "));
  if (state == HIGH) {
    Serial.println(F("HIGH — OK em repouso (premir = ligar outro terminal a GND)"));
  } else {
    Serial.println(F("LOW — curto a GND ou botao premido / pinagem errada"));
  }
}

void readDht() {
  unsigned long now = millis();
  if (now - lastReadMs < READ_INTERVAL_MS) return;
  lastReadMs = now;

  // KY-015 (DHT11): humidade PRIMEIRO, temperatura depois (mesma leitura interna)
  for (uint8_t attempt = 0; attempt < 3; attempt++) {
    envHum = dhtSensor.readHumidity();
    envTemp = dhtSensor.readTemperature();
    if (!isnan(envTemp) && !isnan(envHum)) break;
    delay(250);
  }

  pushSample(envTemp, envHum);

  if (isnan(envTemp) || isnan(envHum)) {
    Serial.print(F("KY-015: leitura invalida (pin "));
    Serial.print(PIN_DHT);
    Serial.println(F(") — verifica GND / S / VCC do modulo"));
    return;
  }

  Serial.print(F("KY-015: "));
  Serial.print(envTemp, 1);
  Serial.print(F(" C | "));
  Serial.print(envHum, 1);
  Serial.println(F(" %"));
}

void goToScreen(int delta) {
  int next = (int)currentScreen + delta;
  if (next < 0) {
    next = SCREEN_COUNT - 1;
  } else if (next >= SCREEN_COUNT) {
    next = 0;
  }
  currentScreen = (ScreenId)next;
  lastScreenChangeMs = millis();
  logScreenChange();
}

void readButtons() {
  if (readDebouncedButtonPressed(PIN_BTN_NEXT, btnNextLastRaw, btnNextStable,
                                 btnNextLastChangeMs)) {
    Serial.println(F("BTN NEXT premido"));
    goToScreen(+1);
  }
  if (readDebouncedButtonPressed(PIN_BTN_PREV, btnPrevLastRaw, btnPrevStable,
                                 btnPrevLastChangeMs)) {
    Serial.println(F("BTN PREV premido"));
    goToScreen(-1);
  }
}

void advanceScreenAuto() {
  unsigned long now = millis();
  if (now - lastScreenChangeMs < SCREEN_INTERVAL_MS) return;

  goToScreen(+1);
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_BTN_PREV, INPUT_PULLUP);

  Serial.begin(9600);
  delay(500);
  Serial.println();
  Serial.println(F("===== TESTE KY-015 + OLED ====="));

  diagnoseDhtPin();
  diagnoseBtnPin(PIN_BTN_NEXT, F("BTN NEXT"));
  diagnoseBtnPin(PIN_BTN_PREV, F("BTN PREV"));
  dhtSensor.begin();
  delay(2000);  // KY-015 (DHT11) precisa ~2s apos ligar

  Wire.begin();
  Wire.setClock(100000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("Falha OLED — verifica SDA/SCL e alimentacao"));
    while (true) delay(10);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  renderScreen();

  Serial.println(F("OLED OK — auto 5s ou botoes NEXT/PREV"));
  Serial.println(F("Leituras KY-015 a cada 2s"));
  Serial.println(F("Mega SDA=20 SCL=21 KY015_S=2 NEXT=3 PREV=4"));
  Serial.println(F("=============================="));
}

void loop() {
  readDht();
  readButtons();
  advanceScreenAuto();
  renderScreen();
  digitalWrite(13, !digitalRead(13));
  delay(50);
}
