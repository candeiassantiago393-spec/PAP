/*
  Teste bancada — MQ-2 / MQ-135 (sensor gás) + OLED SSD1306
  Mega: SDA=20, SCL=21
        MQ AOUT -> A1   DO -> pin 5 (opcional)
        BTN NEXT pin 3 | BTN PREV pin 4 | LED amarelo pin 8

  Ecrãs (NEXT / PREV):
    0 HOME | 1 GAS | 2 GRAF | 3 DIAG (raw AO, DO, baseline)

  Detecção: DO digital (LOW=gás) OU subida do AO vs baseline calibrada.
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define OLED_RESET -1

#define PIN_GAS_AO A1
#define PIN_GAS_DO 5
#define PIN_BTN_NEXT 3
#define PIN_BTN_PREV 4
#define PIN_LED_YELLOW 8

#define GAS_LIMIT 50
#define GAS_AO_RISE_FULL 300
#define GAS_DO_ACTIVE_LOW 1
#define GAS_ALARM_BLINK_MS 250
#define READ_INTERVAL_MS 300
#define DEBOUNCE_MS 35
#define MQ_WARMUP_MS 30000
#define ADC_MAX 1023

#define SPARK_LEN 48
#define SPARK_X 4
#define SPARK_Y 14
#define SPARK_W 120
#define SPARK_H 42

enum ScreenId {
  SCREEN_HOME = 0,
  SCREEN_GAS = 1,
  SCREEN_GRAPH = 2,
  SCREEN_DIAG = 3,
  SCREEN_COUNT = 4
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

ScreenId currentScreen = SCREEN_HOME;
int gasRaw = 0;
int gasBaseline = 0;
int gasRise = 0;
int gasPercent = 0;
bool gasDoActive = false;
bool gasAlert = false;
unsigned long lastReadMs = 0;
unsigned long lastGasBlinkMs = 0;
bool gasBlinkState = false;

int gasHistory[SPARK_LEN];
uint8_t historyIdx = 0;
uint8_t historyCount = 0;

bool btnNextLastRaw = HIGH;
bool btnNextStable = HIGH;
unsigned long btnNextLastChangeMs = 0;

bool btnPrevLastRaw = HIGH;
bool btnPrevStable = HIGH;
unsigned long btnPrevLastChangeMs = 0;

const char *screenNames[] = {"HOME", "GAS", "GRAF", "DIAG"};

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

bool readGasDo() {
  bool raw = digitalRead(PIN_GAS_DO);
#if GAS_DO_ACTIVE_LOW
  return raw == LOW;
#else
  return raw == HIGH;
#endif
}

int aoRiseToPercent(int rise) {
  if (rise <= 0) return 0;
  return constrain(map(rise, 0, GAS_AO_RISE_FULL, 0, 100), 0, 100);
}

void calibrateBaseline() {
  long sum = 0;
  for (int i = 0; i < 24; i++) {
    sum += analogRead(PIN_GAS_AO);
    delay(40);
  }
  gasBaseline = (int)(sum / 24);
}

void updateGas() {
  gasRaw = analogRead(PIN_GAS_AO);
  gasDoActive = readGasDo();
  gasRise = gasRaw - gasBaseline;
  if (gasRise < 0) gasRise = 0;

  int pctAo = aoRiseToPercent(gasRise);
  int pctMap = map(gasRaw, 0, ADC_MAX, 0, 100);
  gasPercent = max(pctAo, pctMap);
  if (gasDoActive) gasPercent = 100;

  gasAlert = gasDoActive || (gasPercent >= GAS_LIMIT);
}

void pushHistory(int pct) {
  gasHistory[historyIdx] = pct;
  historyIdx = (historyIdx + 1) % SPARK_LEN;
  if (historyCount < SPARK_LEN) historyCount++;
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

void drawHomeScreen() {
  display.clearDisplay();
  drawHeader("HOME");
  drawCenteredText(22, "candeias.dev", 2);
  drawCenteredText(44, "studio", 1);
  display.setCursor(4, 54);
  display.print(F("NEXT/PREV ecras"));
  display.setCursor(4, 62);
  display.print(F("AO A1 | DO pin5"));
  drawScreenDots();
  display.display();
}

void drawGasScreen() {
  char line[24];
  const int barX = 4;
  const int barY = 22;
  const int barW = 120;
  const int barH = 10;
  const int innerW = barW - 2;
  const int markX = barX + 1 + (innerW * GAS_LIMIT) / 100;

  display.clearDisplay();
  drawHeader("GAS");
  display.setCursor(4, 14);
  display.print(gasDoActive ? F("DO: GAS detetado") : F("DO: sem gas"));

  display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);
  int fillW = map(constrain(gasPercent, 0, 100), 0, 100, 0, innerW);
  if (fillW > 0) display.fillRect(barX + 1, barY + 1, fillW, barH - 2, SSD1306_WHITE);
  display.drawFastVLine(markX, barY + barH, 3, SSD1306_WHITE);

  snprintf(line, sizeof(line), "%d%%", gasPercent);
  drawTextRight(40, line);
  display.setCursor(4, 40);
  display.print(F("Limite 50%"));
  display.setCursor(4, 52);
  display.print(F("Estado: "));
  display.print(gasAlert ? F("FUGA!") : F("OK"));
  drawScreenDots();
  display.display();
}

void drawGraphScreen() {
  display.clearDisplay();
  drawHeader("GRAF GAS");
  display.drawRect(SPARK_X, SPARK_Y, SPARK_W, SPARK_H, SSD1306_WHITE);

  if (historyCount >= 2) {
    int prevX = -1, prevY = -1;
    for (uint8_t n = 0; n < historyCount; n++) {
      uint8_t idx = (historyIdx + SPARK_LEN - historyCount + n) % SPARK_LEN;
      int val = constrain(gasHistory[idx], 0, 100);
      int x = SPARK_X + 1 + (n * (SPARK_W - 2)) / (SPARK_LEN - 1);
      int y = SPARK_Y + SPARK_H - 2 - (val * (SPARK_H - 2)) / 100;
      if (prevX >= 0) display.drawLine(prevX, prevY, x, y, SSD1306_WHITE);
      prevX = x;
      prevY = y;
    }
  } else {
    display.setCursor(12, 30);
    display.print(F("A recolher..."));
  }

  display.setCursor(4, 58);
  display.print(F("Atual: "));
  display.print(gasPercent);
  display.print(F("%"));
  drawScreenDots();
  display.display();
}

void drawDiagScreen() {
  display.clearDisplay();
  drawHeader("DIAG MQ");
  display.setCursor(4, 14);
  display.print(F("AO A1 raw: "));
  display.print(gasRaw);
  display.setCursor(4, 24);
  display.print(F("Baseline: "));
  display.print(gasBaseline);
  display.setCursor(4, 34);
  display.print(F("Subida: "));
  display.print(gasRise);
  display.setCursor(4, 44);
  display.print(F("DO pin5: "));
  display.print(digitalRead(PIN_GAS_DO) == LOW ? F("LOW") : F("HIGH"));
  display.setCursor(4, 54);
  display.print(F("DO gas: "));
  display.print(gasDoActive ? F("SIM") : F("NAO"));
  drawScreenDots();
  display.display();
}

void drawGasAlert() {
  char line[20];
  display.clearDisplay();
  display.drawRect(2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, SSD1306_WHITE);
  if (gasBlinkState) {
    drawCenteredText(22, "GAS!!", 2);
    drawCenteredText(40, "FUGA DE", 2);
  } else {
    drawCenteredText(31, "FUGA DE GAS!!", 1);
  }
  snprintf(line, sizeof(line), "Nivel: %d%%", gasPercent);
  drawCenteredText(48, line, 1);
  display.display();
}

void renderScreen() {
  if (gasAlert) {
    drawGasAlert();
    return;
  }
  switch (currentScreen) {
    case SCREEN_HOME: drawHomeScreen(); break;
    case SCREEN_GAS: drawGasScreen(); break;
    case SCREEN_GRAPH: drawGraphScreen(); break;
    case SCREEN_DIAG: drawDiagScreen(); break;
    default: drawHomeScreen(); break;
  }
}

void goToScreen(int delta) {
  if (gasAlert) return;
  int next = (int)currentScreen + delta;
  if (next < 0) next = SCREEN_COUNT - 1;
  else if (next >= SCREEN_COUNT) next = 0;
  currentScreen = (ScreenId)next;
  Serial.print(F("Ecra: "));
  Serial.println(screenNames[currentScreen]);
  renderScreen();
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

void updateGasOutputs() {
  if (!gasAlert) {
    digitalWrite(PIN_LED_YELLOW, LOW);
    return;
  }
  unsigned long now = millis();
  if (now - lastGasBlinkMs >= GAS_ALARM_BLINK_MS) {
    lastGasBlinkMs = now;
    gasBlinkState = !gasBlinkState;
    digitalWrite(PIN_LED_YELLOW, gasBlinkState ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(9600);
  delay(400);

  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_BTN_PREV, INPUT_PULLUP);
  pinMode(PIN_GAS_DO, INPUT_PULLUP);
  pinMode(PIN_LED_YELLOW, OUTPUT);
  digitalWrite(PIN_LED_YELLOW, LOW);

  Wire.begin();
  Wire.setClock(100000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("ERRO: OLED nao encontrado"));
    for (;;) delay(1000);
  }

  Serial.println(F("===== TESTE MQ GAS + OLED ====="));
  Serial.println(F("AO -> A1 | DO -> pin 5"));
  Serial.println(F("NEXT -> 3 | PREV -> 4"));
  Serial.println(F("Aquecimento MQ 30s..."));
  delay(MQ_WARMUP_MS);

  calibrateBaseline();
  Serial.print(F("Baseline AO="));
  Serial.println(gasBaseline);
  Serial.println(F("Aproxima gas/isqueiro: DO LOW ou AO sobe"));
  Serial.println();

  renderScreen();
}

void loop() {
  unsigned long now = millis();

  readButtons();

  if (now - lastReadMs >= READ_INTERVAL_MS) {
    lastReadMs = now;
    updateGas();
    pushHistory(gasPercent);

    Serial.print(F("AO="));
    Serial.print(gasRaw);
    Serial.print(F(" base="));
    Serial.print(gasBaseline);
    Serial.print(F(" rise="));
    Serial.print(gasRise);
    Serial.print(F(" DO="));
    Serial.print(gasDoActive ? F("GAS") : F("-"));
    Serial.print(F(" -> "));
    Serial.print(gasPercent);
    Serial.println(gasAlert ? F("% ALARME") : F("% OK"));

    renderScreen();
  }

  updateGasOutputs();
}
