/*
  KY-015 + MQ gás + OLED + NEXT + buzzer
  Mega: DHT=2 | NEXT=3 | Buzzer=6 | MQ AO=A1 | OLED SDA=20 SCL=21
  Ecrãs: HOME | GRAFICOS | BARRAS | GAS

  Buzzer: liga quando gás >= GAS_LIMIT (50%). Desliga abaixo do limite.
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
#define PIN_BUZZER 6
#define PIN_GAS A1
#define DHTTYPE DHT11

#define GAS_LIMIT 50
#define GAS_AO_RISE_FULL 300
#define TEMP_BAR_MIN 0.0f
#define TEMP_BAR_MAX 50.0f
#define GAS_ALARM_BLINK_MS 250
#define READ_DHT_MS 2000
#define READ_GAS_MS 300
#define DEBOUNCE_MS 35
#define MQ_WARMUP_MS 30000
#define OLED_MIN_REFRESH_MS 1000
#define ADC_MAX 1023
#define SPARK_LEN 48

enum ScreenId {
  SCREEN_HOME = 0,
  SCREEN_GRAPHS = 1,
  SCREEN_BARS_ENV = 2,
  SCREEN_GAS = 3,
  SCREEN_COUNT = 4
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dhtSensor(PIN_DHT, DHTTYPE);

ScreenId currentScreen = SCREEN_HOME;
float envTemp = NAN;
float envHum = NAN;
int gasRaw = 0;
int gasBaseline = 0;
int gasRise = 0;
int gasPercent = 0;
bool gasAlert = false;
bool gasBlinkState = false;

unsigned long lastDhtMs = 0;
unsigned long lastGasMs = 0;
unsigned long lastGasBlinkMs = 0;
unsigned long lastOledDrawMs = 0;
unsigned long btnNextLastChangeMs = 0;
unsigned long btnNextCooldownMs = 0;

float tempHistory[SPARK_LEN];
float humHistory[SPARK_LEN];
uint8_t historyIdx = 0;
uint8_t historyCount = 0;

bool btnNextLastRaw = HIGH;
bool btnNextStable = HIGH;
bool displayDirty = true;

int lastDrawnGasPercent = -1;
bool lastDrawnGasAlert = false;

const char *screenNames[] = {"HOME", "GRAFICOS", "BARRAS", "GAS"};

void floatToStr(char *out, size_t outLen, float value, uint8_t decimals) {
  if (outLen == 0) return;
  char tmp[16];
  dtostrf(value, 0, decimals, tmp);
  const char *p = tmp;
  while (*p == ' ') p++;
  strncpy(out, p, outLen - 1);
  out[outLen - 1] = '\0';
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

bool readNextPressed() {
  bool raw = digitalRead(PIN_BTN_NEXT);
  unsigned long now = millis();
  if (raw != btnNextLastRaw) {
    btnNextLastRaw = raw;
    btnNextLastChangeMs = now;
  }
  if ((now - btnNextLastChangeMs) > DEBOUNCE_MS && raw != btnNextStable) {
    btnNextStable = raw;
    if (btnNextStable == LOW && (now - btnNextCooldownMs) > 180) {
      btnNextCooldownMs = now;
      return true;
    }
  }
  return false;
}

void calibrateGasBaseline() {
  long sum = 0;
  for (int i = 0; i < 24; i++) {
    sum += analogRead(PIN_GAS);
    delay(40);
  }
  gasBaseline = (int)(sum / 24);
}

void updateGas() {
  gasRaw = analogRead(PIN_GAS);
  gasRise = gasRaw - gasBaseline;
  if (gasRise < 0) gasRise = 0;
  gasPercent = constrain(map(gasRise, 0, GAS_AO_RISE_FULL, 0, 100), 0, 100);
  gasAlert = (gasPercent >= GAS_LIMIT);
}

void pushEnvHistory(float temp, float hum) {
  if (!isnan(temp)) tempHistory[historyIdx] = temp;
  if (!isnan(hum)) humHistory[historyIdx] = hum;
  historyIdx = (historyIdx + 1) % SPARK_LEN;
  if (historyCount < SPARK_LEN) historyCount++;
}

float historyAt(const float *buf, uint8_t age) {
  if (historyCount < SPARK_LEN) return buf[age];
  return buf[(historyIdx + age) % SPARK_LEN];
}

int tempToBarPct(float temp) {
  if (isnan(temp)) return 0;
  return constrain((int)((temp - TEMP_BAR_MIN) * 100.0f / (TEMP_BAR_MAX - TEMP_BAR_MIN)), 0, 100);
}

void emitTelemetryJson() {
  Serial.print(F("{\"t\":"));
  if (isnan(envTemp)) Serial.print(F("null"));
  else Serial.print(envTemp, 1);
  Serial.print(F(",\"h\":"));
  if (isnan(envHum)) Serial.print(F("null"));
  else Serial.print(envHum, 1);
  Serial.print(F(",\"gas\":"));
  Serial.print(gasPercent);
  Serial.print(F(",\"gr\":"));
  Serial.print(gasRaw);
  Serial.print(F(",\"gb\":"));
  Serial.print(gasBaseline);
  Serial.print(F(",\"alarm\":"));
  Serial.print(gasAlert ? 1 : 0);
  Serial.print(F(",\"scr\":"));
  Serial.print((int)currentScreen);
  Serial.print(F(",\"ms\":"));
  Serial.print(millis());
  Serial.println(F("}"));
}

void readDht() {
  unsigned long now = millis();
  if (now - lastDhtMs < READ_DHT_MS) return;
  lastDhtMs = now;

  envHum = dhtSensor.readHumidity();
  envTemp = dhtSensor.readTemperature();
  if (isnan(envTemp) || isnan(envHum)) return;

  pushEnvHistory(envTemp, envHum);
  displayDirty = true;
  emitTelemetryJson();
}

void drawHeader(const char *title) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 0);
  display.print(title);
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);
}

void drawTextRight(int y, const char *text) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  display.setCursor(124 - w, y);
  display.print(text);
}

void drawCenteredText(int y, const char *text, uint8_t textSize) {
  display.setTextSize(textSize);
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y);
  display.print(text);
}

void drawScreenDots() {
  int cx = SCREEN_WIDTH - 4;
  for (int i = SCREEN_COUNT - 1; i >= 0; i--) {
    if (i == (int)currentScreen) display.fillCircle(cx, 3, 1, SSD1306_WHITE);
    else display.drawCircle(cx, 3, 1, SSD1306_WHITE);
    cx -= 5;
  }
}

void drawMiniSparkline(int gx, int gy, int gw, int gh, const float *buf) {
  display.drawRect(gx, gy, gw, gh, SSD1306_WHITE);
  if (historyCount < 2) {
    display.setCursor(gx + 4, gy + 4);
    display.print(F("..."));
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

  int prevPx = -1, prevPy = -1;
  for (uint8_t i = 0; i < historyCount; i++) {
    float v = historyAt(buf, i);
    int px = gx + 1 + (i * (gw - 2)) / (historyCount - 1);
    int py = gy + gh - 2 - (int)((v - minV) * (gh - 2) / (maxV - minV));
    py = constrain(py, gy + 1, gy + gh - 2);
    if (prevPx >= 0) display.drawLine(prevPx, prevPy, px, py, SSD1306_WHITE);
    prevPx = px;
    prevPy = py;
  }
}

void drawHorizontalBar(int x, int y, int w, int h, int pct, const char *label) {
  display.setCursor(x, y - 8);
  display.print(label);
  display.drawRect(x, y, w, h, SSD1306_WHITE);
  int fillW = map(constrain(pct, 0, 100), 0, 100, 0, w - 2);
  if (fillW > 0) display.fillRect(x + 1, y + 1, fillW, h - 2, SSD1306_WHITE);
}

void drawWarmupScreen() {
  display.clearDisplay();
  drawHeader("CENTRAL");
  drawCenteredText(24, "Aquecer MQ", 1);
  drawCenteredText(40, "30 seg...", 1);
  display.display();
}

void drawHomeScreen() {
  display.clearDisplay();
  drawHeader("HOME");
  drawCenteredText(22, "candeias.dev", 2);
  drawCenteredText(44, "studio", 1);
  display.setCursor(4, 54);
  display.print(F("NEXT -> ecras"));
  if (gasAlert) {
    display.setCursor(4, 62);
    display.print(F("!! GAS !!"));
  }
  drawScreenDots();
  display.display();
}

void drawGraphsScreen() {
  display.clearDisplay();
  drawHeader("GRAFICOS");
  display.setCursor(4, 12);
  display.print(F("Temp C"));
  drawMiniSparkline(4, 20, 120, 18, tempHistory);
  display.setCursor(4, 42);
  display.print(F("Hum %"));
  drawMiniSparkline(4, 50, 120, 12, humHistory);
  drawScreenDots();
  display.display();
}

void drawBarsEnvScreen() {
  char line[20];
  char tStr[8];
  char hStr[8];
  display.clearDisplay();
  drawHeader("BARRAS");

  if (isnan(envTemp) || isnan(envHum)) {
    display.setCursor(4, 28);
    display.print(F("A ler KY-015..."));
  } else {
    floatToStr(tStr, sizeof(tStr), envTemp, 1);
    floatToStr(hStr, sizeof(hStr), envHum, 1);
    snprintf(line, sizeof(line), "Temp %s C", tStr);
    drawHorizontalBar(4, 28, 120, 10, tempToBarPct(envTemp), line);
    snprintf(line, sizeof(line), "Hum %s %%", hStr);
    drawHorizontalBar(4, 52, 120, 10, constrain((int)envHum, 0, 100), line);
  }

  drawScreenDots();
  display.display();
}

void drawGasScreen() {
  char line[24];
  display.clearDisplay();
  drawHeader(gasAlert && gasBlinkState ? "!! GAS !!" : "GAS");
  display.setCursor(4, 14);
  display.print(F("Nivel (A1)"));
  display.drawRect(4, 28, 120, 12, SSD1306_WHITE);
  int fillW = map(constrain(gasPercent, 0, 100), 0, 100, 0, 118);
  if (fillW > 0) display.fillRect(5, 29, fillW, 10, SSD1306_WHITE);
  display.drawFastVLine(4 + 1 + (118 * GAS_LIMIT) / 100, 28, 16, SSD1306_WHITE);
  snprintf(line, sizeof(line), "%d%%", gasPercent);
  drawTextRight(14, line);
  display.setCursor(4, 46);
  snprintf(line, sizeof(line), "Limite %d%%", GAS_LIMIT);
  display.print(line);
  display.setCursor(4, 56);
  display.print(gasAlert ? F("FUGA! BUZZER ON") : F("OK"));
  drawScreenDots();
  display.display();
}

void renderScreen() {
  switch (currentScreen) {
    case SCREEN_HOME: drawHomeScreen(); break;
    case SCREEN_GRAPHS: drawGraphsScreen(); break;
    case SCREEN_BARS_ENV: drawBarsEnvScreen(); break;
    case SCREEN_GAS: drawGasScreen(); break;
    default: drawHomeScreen(); break;
  }
  lastDrawnGasPercent = gasPercent;
  lastDrawnGasAlert = gasAlert;
  lastOledDrawMs = millis();
  displayDirty = false;
}

void renderScreenIfNeeded(bool force) {
  if (!force && !displayDirty) return;
  unsigned long now = millis();
  if (!force && (now - lastOledDrawMs) < OLED_MIN_REFRESH_MS) return;
  renderScreen();
}

void nextScreen() {
  currentScreen = (ScreenId)(((int)currentScreen + 1) % SCREEN_COUNT);
  Serial.print(F("Ecra: "));
  Serial.println(screenNames[currentScreen]);
  displayDirty = true;
  renderScreenIfNeeded(true);
}

void updateGasAlarm() {
  if (!gasAlert) {
    setBuzzer(false);
    gasBlinkState = false;
    return;
  }

  // Buzzer ligado enquanto gás >= GAS_LIMIT
  setBuzzer(true);

  unsigned long now = millis();
  if (now - lastGasBlinkMs >= GAS_ALARM_BLINK_MS) {
    lastGasBlinkMs = now;
    gasBlinkState = !gasBlinkState;
    if (currentScreen == SCREEN_GAS || currentScreen == SCREEN_HOME) {
      displayDirty = true;
      renderScreenIfNeeded(true);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  setBuzzer(false);

  Wire.begin();
  Wire.setClock(100000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("ERRO: OLED nao encontrado"));
    for (;;) delay(1000);
  }

  dhtSensor.begin();
  delay(2000);

  Serial.println(F("===== DHT + GAS + OLED + NEXT + BUZZER ====="));
  Serial.println(F("Buzzer pin 6 — alarme se gas >= 50%"));
  drawWarmupScreen();
  delay(MQ_WARMUP_MS);

  calibrateGasBaseline();
  Serial.print(F("Baseline gas A1="));
  Serial.println(gasBaseline);

  displayDirty = true;
  renderScreenIfNeeded(true);
}

void loop() {
  if (readNextPressed()) {
    Serial.println(F("BTN NEXT premido"));
    nextScreen();
  }

  readDht();

  if (millis() - lastGasMs >= READ_GAS_MS) {
    lastGasMs = millis();
    bool prevAlert = gasAlert;
    int prevPercent = gasPercent;
    updateGas();
    emitTelemetryJson();

    if (gasAlert != prevAlert || abs(gasPercent - prevPercent) >= 2 ||
        gasPercent != lastDrawnGasPercent) {
      displayDirty = true;
    }
  }

  renderScreenIfNeeded(false);
  updateGasAlarm();
  delay(10);
}
