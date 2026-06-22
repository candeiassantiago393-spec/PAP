#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

#if __has_include("secrets.h")
#include "secrets.h"
#else
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define TELEGRAM_BOT_TOKEN ""
#define TELEGRAM_CHAT_ID ""
#define TELEGRAM_TEST_ON_BOOT 0
#endif

#ifndef TELEGRAM_TEST_ON_BOOT
#define TELEGRAM_TEST_ON_BOOT 0
#endif

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define OLED_RESET -1

#define PIN_BTN_NEXT 32
#define PIN_BTN_OK 33
#define PIN_PIR 27
#define PIN_BUZZER 25
#define PIN_DHT 26

// Estufa v1.3 (adaptado para ESP32)
#define PIN_SOIL 34
#define PIN_LED_RED 16
#define PIN_LED_YELLOW 17
#define PIN_LED_GREEN 18
#define PIN_PUMP 19

#define SOIL_LIMIT_LOW 35
#define SOIL_LIMIT_HIGH 75
#define GREENHOUSE_INTERVAL_MS 1000
#define GREEN_LED_BLINK_MS 300
#define PUMP_MAX_ON_MS 10000
#define PUMP_PAUSE_MS 30000
#define ALARM_LED_BLINK_MS 200

#define DEBOUNCE_MS 35
#define PIR_COOLDOWN_MS 30000
#define FEIRA_INTERVAL_MS 5000
#define FEIRA_IDLE_MS 120000
#define WIFI_CONNECT_TIMEOUT_MS 15000
#define WIFI_RETRY_MS 30000
#define TELEGRAM_MSG_MAX 160

#define SPARK_LEN 48
#define SPARK_X 4
#define SPARK_Y 14
#define SPARK_W 120
#define SPARK_H 42

// QR 31x31 — https://www.instagram.com/candeias.dev
#define QR_W 31
#define QR_H 31
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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

enum ScreenId {
  SCREEN_HOME = 0,
  SCREEN_ENV = 1,
  SCREEN_MOTION = 2,
  SCREEN_PLANTAS = 3,
  SCREEN_GRAPH = 4,
  SCREEN_INSTAGRAM = 5,
  SCREEN_COUNT = 6
};

ScreenId currentScreen = SCREEN_HOME;
ScreenId previousScreen = SCREEN_HOME;

bool isTransitioning = false;
int transitionOffset = 0;
const int transitionStep = 16;
const int transitionIntervalMs = 20;
unsigned long lastTransitionTick = 0;

bool alertActive = false;

float envTemp = NAN;
float envHum = NAN;
unsigned long lastEnvTick = 0;

float tempHistory[SPARK_LEN];
uint8_t tempHistoryIdx = 0;
uint8_t tempHistoryCount = 0;

int soilPercent = 0;
const char *soilState = "----";
bool pumpOn = false;
bool pumpRunning = false;
int pumpSecondsLeft = 0;
unsigned long pumpStartedMs = 0;
unsigned long pumpPauseUntilMs = 0;
bool soilBuzzerOn = false;
unsigned long lastGreenhouseTick = 0;
unsigned long lastGreenBlinkMs = 0;
bool greenBlinkState = false;
unsigned long lastAlarmLedBlinkMs = 0;
bool alarmLedOn = false;

bool btnNextLastRaw = HIGH;
bool btnNextStable = HIGH;
unsigned long btnNextLastChangeMs = 0;

bool btnOkLastRaw = HIGH;
bool btnOkStable = HIGH;
unsigned long btnOkLastChangeMs = 0;

bool pirLast = LOW;
unsigned long pirNextAllowedMs = 0;

bool feiraMode = true;
unsigned long lastFeiraStepMs = 0;
unsigned long lastButtonActivityMs = 0;

bool wifiReady = false;
unsigned long lastWifiAttemptMs = 0;
bool telegramPending = false;
char telegramPendingMsg[TELEGRAM_MSG_MAX];
bool wasSoilDry = false;
bool soilDryTelegramSent = false;

bool telegramConfigured() {
  return (TELEGRAM_BOT_TOKEN[0] != '\0' && TELEGRAM_CHAT_ID[0] != '\0');
}

bool wifiConfigured() {
  return (WIFI_SSID[0] != '\0');
}

String urlEncode(const String &value) {
  String encoded;
  encoded.reserve(value.length() + 8);
  const char *hex = "0123456789ABCDEF";

  for (size_t i = 0; i < value.length(); i++) {
    const char c = value.charAt(i);
    if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
      encoded += c;
    } else if (c == ' ') {
      encoded += '+';
    } else {
      encoded += '%';
      encoded += hex[(c >> 4) & 0xF];
      encoded += hex[c & 0xF];
    }
  }
  return encoded;
}

bool sendTelegramNow(const char *message) {
  if (!telegramConfigured() || !wifiReady || WiFi.status() != WL_CONNECTED) {
    return false;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  const String url =
      String("https://api.telegram.org/bot") + TELEGRAM_BOT_TOKEN + "/sendMessage";

  if (!http.begin(client, url)) {
    Serial.println("Telegram: falha ao iniciar HTTP");
    return false;
  }

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  const String body = String("chat_id=") + TELEGRAM_CHAT_ID +
                      "&text=" + urlEncode(String(message));

  const int code = http.POST(body);
  Serial.printf("Telegram HTTP %d: %s\n", code, message);
  http.end();
  return (code == 200);
}

void requestTelegram(const char *message) {
  if (!telegramConfigured()) return;
  strncpy(telegramPendingMsg, message, TELEGRAM_MSG_MAX - 1);
  telegramPendingMsg[TELEGRAM_MSG_MAX - 1] = '\0';
  telegramPending = true;
}

void processTelegram() {
  if (!telegramPending) return;
  if (!wifiReady || WiFi.status() != WL_CONNECTED) return;

  if (sendTelegramNow(telegramPendingMsg)) {
    telegramPending = false;
    telegramPendingMsg[0] = '\0';
  }
}

void initWiFi() {
  if (!wifiConfigured()) {
    Serial.println("Wi-Fi: desativado (sem secrets.h / WIFI_SSID)");
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(100);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.printf("Wi-Fi a ligar (%s)...\n", WIFI_SSID);

  const unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - start) < WIFI_CONNECT_TIMEOUT_MS) {
    delay(250);
    Serial.print('.');
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    wifiReady = true;
    Serial.print("Wi-Fi OK: ");
    Serial.println(WiFi.localIP());
  } else {
    wifiReady = false;
    Serial.println("Wi-Fi: falhou (verifica SSID/password)");
  }

  lastWifiAttemptMs = millis();
}

void maintainWiFi() {
  if (!wifiConfigured()) return;

  if (WiFi.status() == WL_CONNECTED) {
    wifiReady = true;
    return;
  }

  wifiReady = false;
  const unsigned long now = millis();
  if (now - lastWifiAttemptMs < WIFI_RETRY_MS) return;

  lastWifiAttemptMs = now;
  Serial.println("Wi-Fi: a religar...");
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void setBuzzer(bool on) {
  if (on) {
    noTone(PIN_BUZZER);
    digitalWrite(PIN_BUZZER, HIGH);
  } else {
    digitalWrite(PIN_BUZZER, LOW);
    noTone(PIN_BUZZER);
  }
}

void drawHeader(const char *title, int xOffset) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2 + xOffset, 0);
  display.print(title);
  display.drawLine(0 + xOffset, 10, 127 + xOffset, 10, SSD1306_WHITE);
}

void drawHomeScreen(int xOffset) {
  drawHeader("HOME", xOffset);
  display.setTextSize(1);
  display.setCursor(22 + xOffset, 22);
  display.print("candeias.dev");
  display.setCursor(46 + xOffset, 38);
  display.print("studio");
}

void drawEnvScreen(int xOffset) {
  char line[32];
  drawHeader("AMBIENTE", xOffset);
  display.setTextSize(1);

  if (isnan(envTemp) || isnan(envHum)) {
    display.setCursor(4 + xOffset, 24);
    display.print("A ler DHT22...");
    display.setCursor(4 + xOffset, 38);
    display.print("Aguarde 2s");
    return;
  }

  snprintf(line, sizeof(line), "Temperatura: %.1f C", envTemp);
  display.setCursor(4 + xOffset, 20);
  display.print(line);

  snprintf(line, sizeof(line), "Humidade:    %.1f %%", envHum);
  display.setCursor(4 + xOffset, 34);
  display.print(line);
}

void drawMotionScreen(int xOffset) {
  char line[24];
  drawHeader("MOVIMENTO", xOffset);
  display.setTextSize(1);
  display.setCursor(4 + xOffset, 18);
  display.print("Sensor PIR ativo");

  int cooldownSec = 0;
  if (millis() < pirNextAllowedMs) {
    cooldownSec = (pirNextAllowedMs - millis() + 999) / 1000;
  }

  if (cooldownSec > 0) {
    snprintf(line, sizeof(line), "Cooldown: %ds", cooldownSec);
    display.setCursor(4 + xOffset, 32);
    display.print(line);
    display.setCursor(4 + xOffset, 46);
    display.print("Aguarda p/ rearmar");
  } else {
    display.setCursor(4 + xOffset, 36);
    display.print("Aguardando deteccao");
  }
}

void drawTextCenteredAt(int centerX, int y, const char *text) {
  display.setTextSize(1);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  display.setCursor(centerX - (w / 2), y);
  display.print(text);
}

void drawTextRight(int xOffset, int y, const char *text) {
  display.setTextSize(1);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  display.setCursor(124 + xOffset - w, y);
  display.print(text);
}

void drawSoilBar(int x, int y, int w, int h, int percent) {
  display.drawRect(x, y, w, h, SSD1306_WHITE);
  int innerW = w - 2;
  int fillW = map(constrain(percent, 0, 100), 0, 100, 0, innerW);
  if (fillW > 0) {
    display.fillRect(x + 1, y + 1, fillW, h - 2, SSD1306_WHITE);
  }
  int markLow = x + 1 + (innerW * SOIL_LIMIT_LOW) / 100;
  int markHigh = x + 1 + (innerW * SOIL_LIMIT_HIGH) / 100;
  display.drawFastVLine(markLow, y + h, 3, SSD1306_WHITE);
  display.drawFastVLine(markHigh, y + h, 3, SSD1306_WHITE);
}

void pushTempSample(float temp) {
  if (isnan(temp)) return;
  tempHistory[tempHistoryIdx] = temp;
  tempHistoryIdx = (tempHistoryIdx + 1) % SPARK_LEN;
  if (tempHistoryCount < SPARK_LEN) tempHistoryCount++;
}

float tempHistoryAt(uint8_t age) {
  if (tempHistoryCount < SPARK_LEN) {
    return tempHistory[age];
  }
  return tempHistory[(tempHistoryIdx + age) % SPARK_LEN];
}

void drawGraphScreen(int xOffset) {
  char line[24];
  const int gx = SPARK_X + xOffset;
  const int gy = SPARK_Y;
  const int innerW = SPARK_W - 2;
  const int innerH = SPARK_H - 2;

  drawHeader("GRAFICO", xOffset);
  display.drawRect(gx, gy, SPARK_W, SPARK_H, SSD1306_WHITE);

  if (tempHistoryCount < 2) {
    display.setCursor(gx + 18, gy + 16);
    display.print("A recolher...");
    if (!isnan(envTemp)) {
      snprintf(line, sizeof(line), "Atual: %.1f C", envTemp);
      display.setCursor(gx + 18, gy + 28);
      display.print(line);
    }
    return;
  }

  float minT = tempHistoryAt(0);
  float maxT = tempHistoryAt(0);
  for (uint8_t i = 1; i < tempHistoryCount; i++) {
    float v = tempHistoryAt(i);
    if (v < minT) minT = v;
    if (v > maxT) maxT = v;
  }
  if ((maxT - minT) < 1.0f) {
    minT -= 0.5f;
    maxT += 0.5f;
  }

  const int plotX = gx + 1;
  const int plotY = gy + 1;
  int prevPx = -1;
  int prevPy = -1;

  for (uint8_t i = 0; i < tempHistoryCount; i++) {
    float v = tempHistoryAt(i);
    int px = plotX;
    if (tempHistoryCount > 1) {
      px = plotX + (i * innerW) / (tempHistoryCount - 1);
    }
    int py = plotY + innerH - 1 - (int)((v - minT) * innerH / (maxT - minT));
    py = constrain(py, plotY, plotY + innerH - 1);

    if (prevPx >= 0) {
      display.drawLine(prevPx, prevPy, px, py, SSD1306_WHITE);
    } else {
      display.drawPixel(px, py, SSD1306_WHITE);
    }
    prevPx = px;
    prevPy = py;
  }

  snprintf(line, sizeof(line), "Min:%.0f Max:%.0f", minT, maxT);
  display.setCursor(gx + 2, gy + SPARK_H + 2);
  display.print(line);

  if (!isnan(envTemp)) {
    snprintf(line, sizeof(line), "%.1f C", envTemp);
    drawTextRight(xOffset, gy + SPARK_H + 2, line);
  }
}

void drawInstagramScreen(int xOffset) {
  const int qrX = (SCREEN_WIDTH - QR_W) / 2 + xOffset;
  const int qrY = 6;
  display.drawBitmap(qrX, qrY, instagramQr, QR_W, QR_H, SSD1306_WHITE);
  drawTextCenteredAt(64 + xOffset, 44, "@candeias.dev");
  drawTextCenteredAt(64 + xOffset, 56, "Instagram");
}

void drawPlantasScreen(int xOffset) {
  char line[28];
  const int barX = 4 + xOffset;
  const int barY = 14;
  const int barW = 120;
  const int barH = 8;
  const int innerW = barW - 2;
  const int markLowX = barX + 1 + (innerW * SOIL_LIMIT_LOW) / 100;
  const int markHighX = barX + 1 + (innerW * SOIL_LIMIT_HIGH) / 100;

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(4 + xOffset, 0);
  display.print("Hum.solo");

  display.drawLine(4 + xOffset, 10, 124 + xOffset, 10, SSD1306_WHITE);

  drawSoilBar(barX, barY, barW, barH, soilPercent);

  drawTextCenteredAt(markLowX, barY + barH + 4, "35");
  drawTextCenteredAt(markHighX, barY + barH + 4, "75");

  display.setCursor(4 + xOffset, 38);
  display.print("Estado: ");
  display.print(soilState);
  snprintf(line, sizeof(line), "%d%%", soilPercent);
  drawTextRight(xOffset, 38, line);

  display.setCursor(4 + xOffset, 50);
  if (pumpOn) {
    snprintf(line, sizeof(line), "Bomba: ON (%ds)", pumpSecondsLeft);
    display.print(line);
  } else if (soilPercent < SOIL_LIMIT_LOW && millis() < pumpPauseUntilMs) {
    int pauseSec = (pumpPauseUntilMs - millis() + 999) / 1000;
    if (pauseSec > 0) {
      snprintf(line, sizeof(line), "Bomba: OFF (%ds)", pauseSec);
      display.print(line);
    } else {
      display.print("Bomba:  OFF");
    }
  } else {
    display.print("Bomba:  OFF");
  }
}

void drawScreenDots(ScreenId active) {
  const int y = 3;
  const int spacing = 5;
  int cx = SCREEN_WIDTH - 4;

  for (int i = SCREEN_COUNT - 1; i >= 0; i--) {
    if (i == active) {
      display.fillCircle(cx, y, 1, SSD1306_WHITE);
    } else {
      display.drawCircle(cx, y, 1, SSD1306_WHITE);
    }
    cx -= spacing;
  }
}

void drawCenteredText(int y, const char *text, uint8_t textSize) {
  display.setTextSize(textSize);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(text);
}

void drawIntruderAlert() {
  display.clearDisplay();
  display.drawRect(2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, SSD1306_WHITE);

  drawCenteredText(6, "ALERTA", 1);
  display.drawLine(8, 17, SCREEN_WIDTH - 8, 17, SSD1306_WHITE);

  drawCenteredText(24, "Intruso", 2);
  drawCenteredText(44, "detetado", 1);

  display.drawLine(8, 52, SCREEN_WIDTH - 8, 52, SSD1306_WHITE);
  drawCenteredText(55, "OK: silenciar", 1);

  display.display();
}

void drawScreen(ScreenId screen, int xOffset) {
  switch (screen) {
    case SCREEN_HOME:
      drawHomeScreen(xOffset);
      break;
    case SCREEN_ENV:
      drawEnvScreen(xOffset);
      break;
    case SCREEN_MOTION:
      drawMotionScreen(xOffset);
      break;
    case SCREEN_PLANTAS:
      drawPlantasScreen(xOffset);
      break;
    case SCREEN_GRAPH:
      drawGraphScreen(xOffset);
      break;
    case SCREEN_INSTAGRAM:
      drawInstagramScreen(xOffset);
      break;
    default:
      break;
  }
}

void silenceAlarm() {
  alertActive = false;
  alarmLedOn = false;
  digitalWrite(PIN_LED_RED, LOW);
  pirNextAllowedMs = millis() + PIR_COOLDOWN_MS;
  setBuzzer(false);
}

void noteButtonActivity() {
  feiraMode = false;
  lastButtonActivityMs = millis();
}

void startTransitionTo(ScreenId next) {
  if (isTransitioning || alertActive) return;
  previousScreen = currentScreen;
  currentScreen = next;
  transitionOffset = 0;
  lastTransitionTick = millis();
  isTransitioning = true;
}

void updateFeiraMode() {
  if (alertActive || isTransitioning) return;

  unsigned long now = millis();

  if (!feiraMode && (now - lastButtonActivityMs >= FEIRA_IDLE_MS)) {
    feiraMode = true;
    lastFeiraStepMs = now;
  }

  if (feiraMode && (now - lastFeiraStepMs >= FEIRA_INTERVAL_MS)) {
    ScreenId next = static_cast<ScreenId>((currentScreen + 1) % SCREEN_COUNT);
    startTransitionTo(next);
    lastFeiraStepMs = now;
  }
}

void updateTransition() {
  if (!isTransitioning) return;

  unsigned long now = millis();
  if (now - lastTransitionTick < transitionIntervalMs) return;

  lastTransitionTick = now;
  transitionOffset += transitionStep;
  if (transitionOffset >= SCREEN_WIDTH) {
    transitionOffset = SCREEN_WIDTH;
    isTransitioning = false;
  }
}

void updateAlarmBuzzer() {
  if (alertActive) {
    setBuzzer(true);
  } else if (!soilBuzzerOn) {
    setBuzzer(false);
  }
}

void renderUI() {
  if (alertActive) {
    drawIntruderAlert();
    return;
  }

  display.clearDisplay();
  if (isTransitioning) {
    drawScreen(previousScreen, -transitionOffset);
    drawScreen(currentScreen, SCREEN_WIDTH - transitionOffset);
  } else {
    drawScreen(currentScreen, 0);
    drawScreenDots(currentScreen);
  }
  display.display();
}

bool readDebouncedButtonPressed(int pin, bool &lastRaw, bool &stable, unsigned long &lastChangeMs) {
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

void updateEnvironmentData() {
  unsigned long now = millis();
  if (now - lastEnvTick < 2000) return;
  lastEnvTick = now;

  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  envTemp = data.temperature;
  envHum = data.humidity;
  pushTempSample(envTemp);
}

int readSoilPercent() {
  int raw = analogRead(PIN_SOIL);
  int pct = map(raw, 0, 4095, 0, 100);
  return constrain(pct, 0, 100);
}

void setAllPlantOutputsLow() {
  digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_PUMP, LOW);
}

void updatePumpControl(unsigned long now) {
  pumpOn = false;
  pumpSecondsLeft = 0;

  if (soilPercent >= SOIL_LIMIT_LOW) {
    pumpRunning = false;
    return;
  }

  if (pumpRunning) {
    unsigned long elapsed = now - pumpStartedMs;
    if (elapsed >= PUMP_MAX_ON_MS) {
      pumpRunning = false;
      pumpPauseUntilMs = now + PUMP_PAUSE_MS;
    } else {
      pumpOn = true;
      pumpSecondsLeft = (PUMP_MAX_ON_MS - elapsed + 999) / 1000;
      digitalWrite(PIN_PUMP, HIGH);
      return;
    }
  }

  if (now >= pumpPauseUntilMs) {
    pumpRunning = true;
    pumpStartedMs = now;
    pumpOn = true;
    pumpSecondsLeft = PUMP_MAX_ON_MS / 1000;
    digitalWrite(PIN_PUMP, HIGH);
  }
}

void updateAlarmLed() {
  if (!alertActive) return;

  setAllPlantOutputsLow();

  unsigned long now = millis();
  if (now - lastAlarmLedBlinkMs >= ALARM_LED_BLINK_MS) {
    lastAlarmLedBlinkMs = now;
    alarmLedOn = !alarmLedOn;
    digitalWrite(PIN_LED_RED, alarmLedOn ? HIGH : LOW);
  }
}

void updateGreenhouseOutputs() {
  if (alertActive) return;

  setAllPlantOutputsLow();
  soilBuzzerOn = false;
  noTone(PIN_BUZZER);
  digitalWrite(PIN_BUZZER, LOW);

  unsigned long now = millis();

  if (soilPercent < SOIL_LIMIT_LOW) {
    soilState = "SECO";
    if (!wasSoilDry && !soilDryTelegramSent) {
      requestTelegram("candeias.dev: Solo SECO! Regar plantas.");
      soilDryTelegramSent = true;
    }
    wasSoilDry = true;
    digitalWrite(PIN_LED_RED, HIGH);
    updatePumpControl(now);

    if (now - lastGreenBlinkMs >= GREEN_LED_BLINK_MS) {
      lastGreenBlinkMs = now;
      greenBlinkState = !greenBlinkState;
    }
    digitalWrite(PIN_LED_GREEN, greenBlinkState ? HIGH : LOW);

    tone(PIN_BUZZER, 2000);
    soilBuzzerOn = true;
  } else if (soilPercent <= SOIL_LIMIT_HIGH) {
    soilState = "IDEAL";
    wasSoilDry = false;
    soilDryTelegramSent = false;
    pumpRunning = false;
    pumpOn = false;
    greenBlinkState = true;
    digitalWrite(PIN_LED_GREEN, HIGH);
  } else {
    soilState = "MOLHADO";
    wasSoilDry = false;
    soilDryTelegramSent = false;
    pumpRunning = false;
    pumpOn = false;
    digitalWrite(PIN_LED_YELLOW, HIGH);
    tone(PIN_BUZZER, 500);
    soilBuzzerOn = true;
  }
}

void updateGreenhouse() {
  unsigned long now = millis();
  if (now - lastGreenhouseTick < GREENHOUSE_INTERVAL_MS) return;
  lastGreenhouseTick = now;

  soilPercent = readSoilPercent();
  updateGreenhouseOutputs();
}

void checkMotionAlert() {
  bool pirNow = digitalRead(PIN_PIR);
  unsigned long now = millis();

  if (pirNow == HIGH && pirLast == LOW && now >= pirNextAllowedMs) {
    alertActive = true;
    pirNextAllowedMs = now + PIR_COOLDOWN_MS;
    lastAlarmLedBlinkMs = 0;
    alarmLedOn = false;
    setAllPlantOutputsLow();
    noTone(PIN_BUZZER);
    requestTelegram("candeias.dev: Intruso detetado!");
  }
  pirLast = pirNow;
}

void setup() {
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_BTN_OK, INPUT_PULLUP);
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_SOIL, INPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_PUMP, OUTPUT);

  setBuzzer(false);
  setAllPlantOutputsLow();
  dhtSensor.setup(PIN_DHT, DHTesp::DHT22);

  for (uint8_t i = 0; i < SPARK_LEN; i++) {
    tempHistory[i] = NAN;
  }

  Serial.begin(115200);

  if (!telegramConfigured()) {
    Serial.println("Telegram: desativado (preenche secrets.h)");
  }

  initWiFi();

#if TELEGRAM_TEST_ON_BOOT
  if (telegramConfigured() && wifiReady) {
    requestTelegram("candeias.dev: sistema online (teste).");
  }
#endif

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Falha OLED");
    while (true) delay(10);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(22, 22);
  display.print("candeias.dev");
  display.setCursor(46, 38);
  display.print("studio");
  display.display();
  delay(1400);

  unsigned long bootMs = millis();
  lastFeiraStepMs = bootMs;
  lastButtonActivityMs = bootMs;
  feiraMode = true;
}

void loop() {
  if (readDebouncedButtonPressed(PIN_BTN_OK, btnOkLastRaw, btnOkStable, btnOkLastChangeMs)) {
    noteButtonActivity();
    if (alertActive) {
      silenceAlarm();
      updateGreenhouseOutputs();
    }
  }

  if (readDebouncedButtonPressed(PIN_BTN_NEXT, btnNextLastRaw, btnNextStable, btnNextLastChangeMs)) {
    noteButtonActivity();
    ScreenId next = static_cast<ScreenId>((currentScreen + 1) % SCREEN_COUNT);
    startTransitionTo(next);
  }

  updateFeiraMode();
  updateTransition();
  updateEnvironmentData();
  updateGreenhouse();
  checkMotionAlert();
  maintainWiFi();
  processTelegram();
  updateAlarmLed();
  updateAlarmBuzzer();
  renderUI();

  delay(5);
}
