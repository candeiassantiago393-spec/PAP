/*
  Teste bancada — KY-015 + MQ gás + OLED + NEXT + buzzer
  Mega: KY-015 S=2 | NEXT=3 | Buzzer=6 | MQ AO=A1 | OLED SDA=20 SCL=21

  Ecrãs (NEXT):
    0 HOME
    1 GRAFICOS  — gráfico temp (cima) + gráfico hum (baixo)
    2 BARRAS    — barra temp (cima) + barra hum (baixo)
    3 GAS       — barra gás + alarme buzzer

  Gás: AO A1, calibrado vs baseline. NEXT funciona sempre.
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

unsigned long lastDhtMs = 0;
unsigned long lastGasMs = 0;
unsigned long lastGasBlinkMs = 0;
bool gasBlinkState = false;

float tempHistory[SPARK_LEN];
float humHistory[SPARK_LEN];
uint8_t historyIdx = 0;
uint8_t historyCount = 0;

bool btnNextLastRaw = HIGH;
bool btnNextStable = HIGH;
unsigned long btnNextLastChangeMs = 0;

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
    if (btnNextStable == LOW) return true;
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

int gasRiseToPercent(int rise) {
  if (rise <= 0) return 0;
  return constrain(map(rise, 0, GAS_AO_RISE_FULL, 0, 100), 0, 100);
}

void updateGas() {
  gasRaw = analogRead(PIN_GAS);
  gasRise = gasRaw - gasBaseline;
  if (gasRise < 0) gasRise = 0;
  gasPercent = gasRiseToPercent(gasRise);
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

int humToBarPct(float hum) {
  if (isnan(hum)) return 0;
  return constrain((int)hum, 0, 100);
}

void readDht() {
  unsigned long now = millis();
  if (now - lastDhtMs < READ_DHT_MS) return;
  lastDhtMs = now;

  for (uint8_t attempt = 0; attempt < 3; attempt++) {
    envHum = dhtSensor.readHumidity();
    envTemp = dhtSensor.readTemperature();
    if (!isnan(envTemp) && !isnan(envHum)) break;
    delay(250);
  }

  if (isnan(envTemp) || isnan(envHum)) {
    Serial.println(F("KY-015: leitura invalida"));
    return;
  }

  pushEnvHistory(envTemp, envHum);
  Serial.print(F("KY-015: "));
  Serial.print(envTemp, 1);
  Serial.print(F(" C | "));
  Serial.print(envHum, 1);
  Serial.println(F(" %"));
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
  const int y = 3;
  int cx = SCREEN_WIDTH - 4;
  for (int i = SCREEN_COUNT - 1; i >= 0; i--) {
    if (i == (int)currentScreen) display.fillCircle(cx, y, 1, SSD1306_WHITE);
    else display.drawCircle(cx, y, 1, SSD1306_WHITE);
    cx -= 5;
  }
}

void drawMiniSparkline(int gx, int gy, int gw, int gh, const float *buf, float current) {
  display.drawRect(gx, gy, gw, gh, SSD1306_WHITE);
  const int innerW = gw - 2;
  const int innerH = gh - 2;

  if (historyCount < 2) {
    display.setCursor(gx + 4, gy + (gh / 2) - 4);
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
    int px = gx + 1 + (i * innerW) / (historyCount - 1);
    int py = gy + gh - 2 - (int)((v - minV) * innerH / (maxV - minV));
    py = constrain(py, gy + 1, gy + gh - 2);
    if (prevPx >= 0) display.drawLine(prevPx, prevPy, px, py, SSD1306_WHITE);
    prevPx = px;
    prevPy = py;
  }

  if (!isnan(current)) {
    char vStr[8];
    floatToStr(vStr, sizeof(vStr), current, 1);
    display.setCursor(gx + 2, gy + gh + 1);
    display.print(vStr);
  }
}

void drawHorizontalBar(int x, int y, int w, int h, int pct, const char *label) {
  const int innerW = w - 2;
  display.setCursor(x, y - 8);
  display.print(label);
  display.drawRect(x, y, w, h, SSD1306_WHITE);
  int fillW = map(constrain(pct, 0, 100), 0, 100, 0, innerW);
  if (fillW > 0) display.fillRect(x + 1, y + 1, fillW, h - 2, SSD1306_WHITE);
  char pctStr[6];
  snprintf(pctStr, sizeof(pctStr), "%d%%", pct);
  drawTextRight(y - 1, pctStr);
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
    display.print(F("! GAS !"));
  }
  drawScreenDots();
  display.display();
}

void drawGraphsScreen() {
  display.clearDisplay();
  drawHeader("GRAFICOS");
  display.setCursor(4, 12);
  display.print(F("Temp C"));
  drawMiniSparkline(4, 20, 120, 18, tempHistory, envTemp);
  display.setCursor(4, 42);
  display.print(F("Hum %"));
  drawMiniSparkline(4, 50, 120, 12, humHistory, envHum);
  drawScreenDots();
  display.display();
}

void drawBarsEnvScreen() {
  char line[20];
  display.clearDisplay();
  drawHeader("BARRAS");

  if (isnan(envTemp) || isnan(envHum)) {
    display.setCursor(4, 28);
    display.print(F("A ler KY-015..."));
  } else {
    char tStr[8], hStr[8];
    floatToStr(tStr, sizeof(tStr), envTemp, 1);
    floatToStr(hStr, sizeof(hStr), envHum, 1);
    snprintf(line, sizeof(line), "Temp %s C", tStr);
    drawHorizontalBar(4, 28, 120, 10, tempToBarPct(envTemp), line);
    snprintf(line, sizeof(line), "Hum %s %%", hStr);
    drawHorizontalBar(4, 52, 120, 10, humToBarPct(envHum), line);
  }

  drawScreenDots();
  display.display();
}

void drawGasScreen() {
  char line[24];
  const int barX = 4;
  const int barY = 28;
  const int barW = 120;
  const int barH = 12;
  const int innerW = barW - 2;
  const int markX = barX + 1 + (innerW * GAS_LIMIT) / 100;

  display.clearDisplay();

  if (gasAlert && gasBlinkState) {
    drawHeader("!! GAS !!");
  } else {
    drawHeader("GAS");
  }

  display.setCursor(4, 14);
  display.print(F("Nivel (A1)"));

  display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);
  int fillW = map(constrain(gasPercent, 0, 100), 0, 100, 0, innerW);
  if (fillW > 0) display.fillRect(barX + 1, barY + 1, fillW, barH - 2, SSD1306_WHITE);
  display.drawFastVLine(markX, barY + barH, 4, SSD1306_WHITE);

  snprintf(line, sizeof(line), "%d%%", gasPercent);
  drawTextRight(14, line);
  display.setCursor(4, 46);
  snprintf(line, sizeof(line), "Limite %d%%", GAS_LIMIT);
  display.print(line);
  display.setCursor(4, 56);
  display.print(F("Estado: "));
  display.print(gasAlert ? F("FUGA!") : F("OK"));

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
}

void nextScreen() {
  currentScreen = (ScreenId)(((int)currentScreen + 1) % SCREEN_COUNT);
  Serial.print(F("Ecra: "));
  Serial.println(screenNames[currentScreen]);
  renderScreen();
}

void updateAlarmOutputs() {
  if (!gasAlert) {
    setBuzzer(false);
    gasBlinkState = false;
    return;
  }
  setBuzzer(true);
  unsigned long now = millis();
  if (now - lastGasBlinkMs >= GAS_ALARM_BLINK_MS) {
    lastGasBlinkMs = now;
    gasBlinkState = !gasBlinkState;
    if (currentScreen == SCREEN_GAS) renderScreen();
  }
}

void setup() {
  Serial.begin(9600);
  delay(400);

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

  Serial.println(F("===== DHT + GAS + OLED + NEXT + BUZZER ====="));
  Serial.println(F("DHT2 NEXT3 BZ6 MQ:A1 | Ecras: HOME GRAF BARRAS GAS"));
  Serial.println(F("Aquecimento MQ 30s..."));
  delay(MQ_WARMUP_MS);

  calibrateGasBaseline();
  Serial.print(F("Baseline gas A1="));
  Serial.println(gasBaseline);

  renderScreen();
}

void loop() {
  unsigned long now = millis();

  if (readNextPressed()) {
    Serial.println(F("BTN NEXT premido"));
    nextScreen();
  }

  readDht();

  if (now - lastGasMs >= READ_GAS_MS) {
    lastGasMs = now;
    updateGas();

    Serial.print(F("GAS raw="));
    Serial.print(gasRaw);
    Serial.print(F(" rise="));
    Serial.print(gasRise);
    Serial.print(F(" -> "));
    Serial.print(gasPercent);
    Serial.println(gasAlert ? F("% ALARME") : F("% OK"));

    if (currentScreen == SCREEN_GRAPHS || currentScreen == SCREEN_BARS_ENV ||
        currentScreen == SCREEN_GAS || currentScreen == SCREEN_HOME) {
      renderScreen();
    }
  }

  updateAlarmOutputs();
}
