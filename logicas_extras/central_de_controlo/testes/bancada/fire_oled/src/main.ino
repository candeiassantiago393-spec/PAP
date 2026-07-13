/*
  Teste bancada — KY-026 (sensor fogo) + OLED SSD1306
  Mega: SDA=20, SCL=21
        KY-026 AO -> A2   DO -> pin 5 (OBRIGATORIO ligar DO!)
        BTN NEXT pin 3 | LED verm pin 7

  Ecrãs (NEXT):
    0 HOME | 1 FOGO | 2 GRAF | 3 DIAG (raw AO, DO, baseline)

  Detecção: DO digital (LOW=chama) OU queda do AO vs baseline calibrada no arranque.
*/

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define OLED_RESET -1

#define PIN_FIRE_AO A2
#define PIN_FIRE_DO 5
#define PIN_BTN_NEXT 3
#define PIN_LED_RED 7

#define FIRE_LIMIT 50
#define FIRE_AO_DROP_FULL 350
#define FIRE_DO_ACTIVE_LOW 1
#define FIRE_ALARM_BLINK_MS 150
#define READ_INTERVAL_MS 300
#define DEBOUNCE_MS 35

#define SPARK_LEN 48
#define SPARK_X 4
#define SPARK_Y 14
#define SPARK_W 120
#define SPARK_H 42

enum ScreenId {
  SCREEN_HOME = 0,
  SCREEN_FIRE = 1,
  SCREEN_GRAPH = 2,
  SCREEN_DIAG = 3,
  SCREEN_COUNT = 4
};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

ScreenId currentScreen = SCREEN_HOME;
int fireRaw = 0;
int fireBaseline = 0;
int fireDrop = 0;
int firePercent = 0;
bool fireDoActive = false;
bool fireAlert = false;
unsigned long lastReadMs = 0;
unsigned long lastFireBlinkMs = 0;
bool fireBlinkState = false;

int fireHistory[SPARK_LEN];
uint8_t historyIdx = 0;
uint8_t historyCount = 0;

bool btnNextLastRaw = HIGH;
bool btnNextStable = HIGH;
unsigned long btnNextLastChangeMs = 0;

const char *screenNames[] = {"HOME", "FOGO", "GRAF", "DIAG"};

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

bool readFireDo() {
  bool raw = digitalRead(PIN_FIRE_DO);
#if FIRE_DO_ACTIVE_LOW
  return raw == LOW;
#else
  return raw == HIGH;
#endif
}

int aoDropToPercent(int drop) {
  if (drop <= 0) return 0;
  return constrain(map(drop, 0, FIRE_AO_DROP_FULL, 0, 100), 0, 100);
}

void calibrateBaseline() {
  long sum = 0;
  for (int i = 0; i < 24; i++) {
    sum += analogRead(PIN_FIRE_AO);
    delay(40);
  }
  fireBaseline = (int)(sum / 24);
}

void updateFire() {
  fireRaw = analogRead(PIN_FIRE_AO);
  fireDoActive = readFireDo();
  fireDrop = fireBaseline - fireRaw;
  if (fireDrop < 0) fireDrop = 0;

  int pctAo = aoDropToPercent(fireDrop);
  firePercent = pctAo;
  if (fireDoActive) firePercent = 100;

  fireAlert = fireDoActive || (firePercent >= FIRE_LIMIT);
}

void pushHistory(int pct) {
  fireHistory[historyIdx] = pct;
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
  display.print(F("NEXT -> ecras"));
  display.setCursor(4, 62);
  display.print(F("DO pin5 | AO A2"));
  drawScreenDots();
  display.display();
}

void drawFireScreen() {
  char line[24];
  const int barX = 4;
  const int barY = 22;
  const int barW = 120;
  const int barH = 10;
  const int innerW = barW - 2;
  const int markX = barX + 1 + (innerW * FIRE_LIMIT) / 100;

  display.clearDisplay();
  drawHeader("FOGO");
  display.setCursor(4, 14);
  display.print(fireDoActive ? F("DO: CHAMA detetada") : F("DO: sem chama"));

  display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);
  int fillW = map(constrain(firePercent, 0, 100), 0, 100, 0, innerW);
  if (fillW > 0) display.fillRect(barX + 1, barY + 1, fillW, barH - 2, SSD1306_WHITE);
  display.drawFastVLine(markX, barY + barH, 3, SSD1306_WHITE);

  snprintf(line, sizeof(line), "%d%%", firePercent);
  drawTextRight(40, line);
  display.setCursor(4, 40);
  display.print(F("Estado: "));
  display.print(fireAlert ? F("FOGO!") : F("OK"));
  display.setCursor(4, 52);
  display.print(F("queda AO="));
  display.print(fireDrop);
  drawScreenDots();
  display.display();
}

void drawGraphScreen() {
  display.clearDisplay();
  drawHeader("GRAF FOGO");
  display.drawRect(SPARK_X, SPARK_Y, SPARK_W, SPARK_H, SSD1306_WHITE);

  if (historyCount >= 2) {
    int prevX = -1, prevY = -1;
    for (uint8_t n = 0; n < historyCount; n++) {
      uint8_t idx = (historyIdx + SPARK_LEN - historyCount + n) % SPARK_LEN;
      int val = constrain(fireHistory[idx], 0, 100);
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
  display.print(firePercent);
  display.print(F("%"));
  drawScreenDots();
  display.display();
}

void drawDiagScreen() {
  display.clearDisplay();
  drawHeader("DIAG KY-026");
  display.setCursor(4, 14);
  display.print(F("AO A2 raw: "));
  display.print(fireRaw);
  display.setCursor(4, 24);
  display.print(F("Baseline: "));
  display.print(fireBaseline);
  display.setCursor(4, 34);
  display.print(F("Queda: "));
  display.print(fireDrop);
  display.setCursor(4, 44);
  display.print(F("DO pin5: "));
  display.print(digitalRead(PIN_FIRE_DO) == LOW ? F("LOW") : F("HIGH"));
  display.setCursor(4, 54);
  display.print(F("DO chama: "));
  display.print(fireDoActive ? F("SIM") : F("NAO"));
  display.setCursor(4, 62);
  display.print(F("LED modulo aceso?"));
  drawScreenDots();
  display.display();
}

void drawFireAlert() {
  char line[20];
  display.clearDisplay();
  display.drawRect(2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, SSD1306_WHITE);
  if (fireBlinkState) drawCenteredText(22, "FOGO!!", 2);
  drawCenteredText(31, "FOGO / INCENDIO", 1);
  snprintf(line, sizeof(line), "Nivel: %d%%", firePercent);
  drawCenteredText(48, line, 1);
  display.display();
}

void renderScreen() {
  if (fireAlert) {
    drawFireAlert();
    return;
  }
  switch (currentScreen) {
    case SCREEN_HOME: drawHomeScreen(); break;
    case SCREEN_FIRE: drawFireScreen(); break;
    case SCREEN_GRAPH: drawGraphScreen(); break;
    case SCREEN_DIAG: drawDiagScreen(); break;
    default: drawHomeScreen(); break;
  }
}

void nextScreen() {
  if (fireAlert) return;
  currentScreen = (ScreenId)(((int)currentScreen + 1) % SCREEN_COUNT);
  Serial.print(F("Ecra: "));
  Serial.println(screenNames[currentScreen]);
  renderScreen();
}

void updateFireOutputs() {
  if (!fireAlert) {
    digitalWrite(PIN_LED_RED, LOW);
    return;
  }
  unsigned long now = millis();
  if (now - lastFireBlinkMs >= FIRE_ALARM_BLINK_MS) {
    lastFireBlinkMs = now;
    fireBlinkState = !fireBlinkState;
    digitalWrite(PIN_LED_RED, fireBlinkState ? HIGH : LOW);
  }
}

void setup() {
  Serial.begin(9600);
  delay(400);

  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_FIRE_DO, INPUT_PULLUP);
  pinMode(PIN_LED_RED, OUTPUT);
  digitalWrite(PIN_LED_RED, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("ERRO: OLED nao encontrado"));
    for (;;) delay(1000);
  }

  Serial.println(F("===== TESTE KY-026 + OLED ====="));
  Serial.println(F("AO -> A2 | DO -> pin 5 | NEXT -> 3"));
  Serial.println(F("Calibracao baseline (sem chama)..."));

  calibrateBaseline();
  Serial.print(F("Baseline AO="));
  Serial.println(fireBaseline);
  Serial.println(F("Aproxima isqueiro: DO deve ir LOW ou AO deve descer"));
  Serial.println();

  renderScreen();
}

void loop() {
  unsigned long now = millis();

  if (readNextPressed()) {
    Serial.println(F("BTN NEXT premido"));
    nextScreen();
  }

  if (now - lastReadMs >= READ_INTERVAL_MS) {
    lastReadMs = now;
    updateFire();
    pushHistory(firePercent);

    Serial.print(F("AO="));
    Serial.print(fireRaw);
    Serial.print(F(" base="));
    Serial.print(fireBaseline);
    Serial.print(F(" drop="));
    Serial.print(fireDrop);
    Serial.print(F(" DO="));
    Serial.print(fireDoActive ? F("CHAMA") : F("-"));
    Serial.print(F(" -> "));
    Serial.print(firePercent);
    Serial.println(fireAlert ? F("% ALARME") : F("% OK"));

    renderScreen();
  }

  updateFireOutputs();
}
