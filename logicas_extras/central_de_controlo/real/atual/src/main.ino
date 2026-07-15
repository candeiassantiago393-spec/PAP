/*
  Central de Controlo — bancada Mega (real/atual)
  KY-015 + MQ gás + KY-026 fogo + KY-002 vibração + OLED + NEXT/PREV + buzzer

  Mega: DHT=2 | NEXT=3 | PREV=4 | Buzzer=6 | MQ=A1 | KY-026 AO=A2 DO=5 | KY-002 S=A3 | OLED 20/21
  Ecrãs: HOME | GRAFICOS | BARRAS | GAS | FOGO | SISMO

  Alarmes (prioridade): fogo > sismo > gás (buzzer pin 6)
  KY-026: AO em A2, DO em pin 5 — calibra baseline AO no arranque
  KY-002: S em A3 — calibra nivel em repouso no arranque
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
#define PIN_BUZZER 6
#define PIN_GAS A1
#define PIN_FIRE_AO A2
#define PIN_FIRE_DO 5
#define PIN_QUAKE A3
#define BUZZER_TONE_HZ 2000
#define DHTTYPE DHT11

#define GAS_LIMIT 50
#define FIRE_LIMIT 50
#define QUAKE_LIMIT 50
#define GAS_AO_RISE_FULL 300
#define FIRE_AO_DROP_FULL 350
#define FIRE_DO_ACTIVE_LOW 1
#define TEMP_BAR_MIN 0.0f
#define TEMP_BAR_MAX 50.0f
#define GAS_ALARM_BLINK_MS 250
#define FIRE_ALARM_BLINK_MS 150
#define QUAKE_ALARM_BLINK_MS 250
#define READ_DHT_MS 2000
#define READ_GAS_MS 300
#define READ_FIRE_MS 300
#define READ_QUAKE_MS 100
#define QUAKE_HOLD_MS 2000
#define DEBOUNCE_MS 35
#define BTN_COOLDOWN_MS 180
#define MQ_WARMUP_MS 30000
#define OLED_MIN_REFRESH_MS 1000
#define ADC_MAX 1023
#define SPARK_LEN 48

enum ScreenId {
  SCREEN_HOME = 0,
  SCREEN_GRAPHS = 1,
  SCREEN_BARS_ENV = 2,
  SCREEN_GAS = 3,
  SCREEN_FIRE = 4,
  SCREEN_QUAKE = 5,
  SCREEN_COUNT = 6
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
int fireRaw = 0;
int fireBaseline = 0;
int fireDrop = 0;
int firePercent = 0;
bool fireDoActive = false;
bool fireAlert = false;
bool fireBlinkState = false;
int quakeRaw = 0;
int quakePercent = 0;
bool quakeAlert = false;
bool quakeBlinkState = false;
uint8_t quakeIdleLevel = HIGH;

unsigned long lastDhtMs = 0;
unsigned long lastGasMs = 0;
unsigned long lastFireMs = 0;
unsigned long lastQuakeMs = 0;
unsigned long quakeHoldUntilMs = 0;
unsigned long lastGasBlinkMs = 0;
unsigned long lastFireBlinkMs = 0;
unsigned long lastQuakeBlinkMs = 0;
unsigned long lastOledDrawMs = 0;

float tempHistory[SPARK_LEN];
float humHistory[SPARK_LEN];
uint8_t historyIdx = 0;
uint8_t historyCount = 0;

bool displayDirty = true;
bool mqReady = false;
unsigned long mqReadyAtMs = 0;
bool btnNextLastRaw = HIGH;
bool btnNextStable = HIGH;
unsigned long btnNextLastChangeMs = 0;
unsigned long btnNextCooldownMs = 0;
bool btnPrevLastRaw = HIGH;
bool btnPrevStable = HIGH;
unsigned long btnPrevLastChangeMs = 0;
unsigned long btnPrevCooldownMs = 0;
unsigned long lastTelemetryMs = 0;

int lastDrawnGasPercent = -1;
bool lastDrawnGasAlert = false;
int lastDrawnFirePercent = -1;
bool lastDrawnFireAlert = false;
int lastDrawnQuakePercent = -1;
bool lastDrawnQuakeAlert = false;

const char *screenNames[] = {"HOME", "GRAFICOS", "BARRAS", "GAS", "FOGO", "SISMO"};

void nextScreen();
void prevScreen();
void renderScreen();

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
    tone(PIN_BUZZER, BUZZER_TONE_HZ);
  } else {
    noTone(PIN_BUZZER);
    digitalWrite(PIN_BUZZER, LOW);
  }
}

bool anySensorAlarm() {
  return fireAlert || quakeAlert || gasAlert;
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
  return constrain(map(drop, 0, FIRE_AO_DROP_FULL, 0, 100), 0, 100);
}

bool readDebouncedButtonPressed(int pin, bool &lastRaw, bool &stable,
                                unsigned long &lastChangeMs,
                                unsigned long &cooldownMs) {
  bool raw = digitalRead(pin);
  unsigned long now = millis();
  if (raw != lastRaw) {
    lastRaw = raw;
    lastChangeMs = now;
  }
  if ((now - lastChangeMs) > DEBOUNCE_MS && raw != stable) {
    stable = raw;
    if (stable == LOW && (now - cooldownMs) > BTN_COOLDOWN_MS) {
      cooldownMs = now;
      return true;
    }
  }
  return false;
}

void handleButtons() {
  if (readDebouncedButtonPressed(PIN_BTN_NEXT, btnNextLastRaw, btnNextStable,
                                 btnNextLastChangeMs, btnNextCooldownMs)) {
    Serial.println(F("BTN NEXT premido"));
    nextScreen();
    return;
  }
  if (readDebouncedButtonPressed(PIN_BTN_PREV, btnPrevLastRaw, btnPrevStable,
                                 btnPrevLastChangeMs, btnPrevCooldownMs)) {
    Serial.println(F("BTN PREV premido"));
    prevScreen();
  }
}

void calibrateGasBaseline() {
  long sum = 0;
  for (int i = 0; i < 24; i++) {
    sum += analogRead(PIN_GAS);
    delay(40);
  }
  gasBaseline = (int)(sum / 24);
}

void calibrateFireBaseline() {
  long sum = 0;
  for (int i = 0; i < 24; i++) {
    sum += analogRead(PIN_FIRE_AO);
    delay(40);
  }
  fireBaseline = (int)(sum / 24);
}

void updateGas() {
  gasRaw = analogRead(PIN_GAS);
  gasRise = gasRaw - gasBaseline;
  if (gasRise < 0) gasRise = 0;
  gasPercent = constrain(map(gasRise, 0, GAS_AO_RISE_FULL, 0, 100), 0, 100);
  gasAlert = (gasPercent >= GAS_LIMIT);
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

void calibrateQuakeIdle() {
  int lowCount = 0;
  for (int i = 0; i < 16; i++) {
    if (digitalRead(PIN_QUAKE) == LOW) lowCount++;
    delay(20);
  }
  quakeIdleLevel = (lowCount >= 8) ? LOW : HIGH;
  quakeHoldUntilMs = 0;
}

void updateQuake() {
  unsigned long now = millis();
  bool active = digitalRead(PIN_QUAKE) != quakeIdleLevel;
  if (active) quakeHoldUntilMs = now + QUAKE_HOLD_MS;
  bool inHold = now < quakeHoldUntilMs;
  bool triggered = active || inHold;
  quakeRaw = triggered ? 1 : 0;
  quakePercent = triggered ? 100 : 0;
  quakeAlert = (quakePercent >= QUAKE_LIMIT);
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
  unsigned long now = millis();
  if ((now - lastTelemetryMs) < 250) return;
  lastTelemetryMs = now;

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
  Serial.print(F(",\"fire\":"));
  Serial.print(firePercent);
  Serial.print(F(",\"fr\":"));
  Serial.print(fireRaw);
  Serial.print(F(",\"fbl\":"));
  Serial.print(fireBaseline);
  Serial.print(F(",\"fd\":"));
  Serial.print(fireDrop);
  Serial.print(F(",\"fa\":"));
  Serial.print(fireAlert ? 1 : 0);
  Serial.print(F(",\"qk\":"));
  Serial.print(quakePercent);
  Serial.print(F(",\"qr\":"));
  Serial.print(quakeRaw);
  Serial.print(F(",\"qa\":"));
  Serial.print(quakeAlert ? 1 : 0);
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
  display.setTextSize(1);
  display.setCursor(x, y - 8);
  display.print(label);
  display.drawRect(x, y, w, h, SSD1306_WHITE);
  int fillW = map(constrain(pct, 0, 100), 0, 100, 0, w - 2);
  if (fillW > 0) display.fillRect(x + 1, y + 1, fillW, h - 2, SSD1306_WHITE);
}

void drawWarmupScreen() {
  display.clearDisplay();
  drawHeader("CENTRAL");
  drawCenteredText(22, "Smart Home", 1);
  drawCenteredText(36, "Lab", 2);
  display.setTextSize(1);
  display.setCursor(4, 54);
  display.print(F("Aquecer MQ..."));
  display.display();
}

void drawHomeScreen() {
  display.clearDisplay();
  drawHeader("HOME");
  drawCenteredText(16, "Smart Home", 1);
  drawCenteredText(32, "Lab", 2);
  display.setTextSize(1);
  display.setCursor(4, 50);
  display.print(F("NEXT/PREV ecras"));
  if (fireAlert) {
    display.setCursor(4, 58);
    display.print(F("!! FOGO !!"));
  } else if (quakeAlert) {
    display.setCursor(4, 58);
    display.print(F("!! SISMO !!"));
  } else if (gasAlert) {
    display.setCursor(4, 58);
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
  display.setTextSize(1);
  display.setCursor(4, 14);
  display.print(F("Gas A1"));
  snprintf(line, sizeof(line), "%d%%", gasPercent);
  drawTextRight(14, line);
  display.drawRect(4, 26, 120, 10, SSD1306_WHITE);
  int fillW = map(constrain(gasPercent, 0, 100), 0, 100, 0, 118);
  if (fillW > 0) display.fillRect(5, 27, fillW, 8, SSD1306_WHITE);
  display.drawFastVLine(4 + 1 + (118 * GAS_LIMIT) / 100, 26, 12, SSD1306_WHITE);
  display.setCursor(4, 42);
  snprintf(line, sizeof(line), "Limite %d%%", GAS_LIMIT);
  display.print(line);
  display.setCursor(4, 54);
  display.print(gasAlert ? F("FUGA! BUZZER ON") : F("OK"));
  drawScreenDots();
  display.display();
}

void drawFireScreen() {
  char line[24];
  display.clearDisplay();
  drawHeader(fireAlert && fireBlinkState ? "!! FOGO !!" : "FOGO");
  display.setTextSize(1);
  display.setCursor(4, 14);
  display.print(F("Chama A2"));
  snprintf(line, sizeof(line), "%d%%", firePercent);
  drawTextRight(14, line);
  display.drawRect(4, 26, 120, 10, SSD1306_WHITE);
  int fillW = map(constrain(firePercent, 0, 100), 0, 100, 0, 118);
  if (fillW > 0) display.fillRect(5, 27, fillW, 8, SSD1306_WHITE);
  display.drawFastVLine(4 + 1 + (118 * FIRE_LIMIT) / 100, 26, 12, SSD1306_WHITE);
  display.setCursor(4, 42);
  display.print(fireDoActive ? F("DO: CHAMA") : F("DO: sem chama"));
  display.setCursor(4, 54);
  display.print(fireAlert ? F("FOGO! BUZZER ON") : F("OK"));
  drawScreenDots();
  display.display();
}

void drawQuakeScreen() {
  char line[24];
  display.clearDisplay();
  drawHeader(quakeAlert && quakeBlinkState ? "!! SISMO !!" : "SISMO");
  display.setTextSize(1);
  display.setCursor(4, 14);
  display.print(F("Vibr A3"));
  snprintf(line, sizeof(line), "%d%%", quakePercent);
  drawTextRight(14, line);
  display.drawRect(4, 26, 120, 10, SSD1306_WHITE);
  int fillW = map(constrain(quakePercent, 0, 100), 0, 100, 0, 118);
  if (fillW > 0) display.fillRect(5, 27, fillW, 8, SSD1306_WHITE);
  display.drawFastVLine(4 + 1 + (118 * QUAKE_LIMIT) / 100, 26, 12, SSD1306_WHITE);
  display.setCursor(4, 42);
  snprintf(line, sizeof(line), "Limite %d%%", QUAKE_LIMIT);
  display.print(line);
  display.setCursor(4, 54);
  display.print(quakeAlert ? F("SISMO! BUZZER ON") : F("OK"));
  drawScreenDots();
  display.display();
}

void renderScreen() {
  switch (currentScreen) {
    case SCREEN_HOME: drawHomeScreen(); break;
    case SCREEN_GRAPHS: drawGraphsScreen(); break;
    case SCREEN_BARS_ENV: drawBarsEnvScreen(); break;
    case SCREEN_GAS: drawGasScreen(); break;
    case SCREEN_FIRE: drawFireScreen(); break;
    case SCREEN_QUAKE: drawQuakeScreen(); break;
    default: drawHomeScreen(); break;
  }
  lastDrawnGasPercent = gasPercent;
  lastDrawnGasAlert = gasAlert;
  lastDrawnFirePercent = firePercent;
  lastDrawnFireAlert = fireAlert;
  lastDrawnQuakePercent = quakePercent;
  lastDrawnQuakeAlert = quakeAlert;
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

void prevScreen() {
  int prev = (int)currentScreen - 1;
  if (prev < 0) prev = SCREEN_COUNT - 1;
  currentScreen = (ScreenId)prev;
  Serial.print(F("Ecra: "));
  Serial.println(screenNames[currentScreen]);
  displayDirty = true;
  renderScreenIfNeeded(true);
}

void updateAlarms() {
  unsigned long now = millis();
  bool alarm = anySensorAlarm();

  setBuzzer(alarm);

  if (fireAlert) {
    if (now - lastFireBlinkMs >= FIRE_ALARM_BLINK_MS) {
      lastFireBlinkMs = now;
      fireBlinkState = !fireBlinkState;
      if (currentScreen == SCREEN_FIRE || currentScreen == SCREEN_HOME) {
        displayDirty = true;
      }
    }
  } else {
    fireBlinkState = false;
  }

  if (quakeAlert) {
    if (now - lastQuakeBlinkMs >= QUAKE_ALARM_BLINK_MS) {
      lastQuakeBlinkMs = now;
      quakeBlinkState = !quakeBlinkState;
      if (currentScreen == SCREEN_QUAKE || currentScreen == SCREEN_HOME) {
        displayDirty = true;
      }
    }
  } else {
    quakeBlinkState = false;
  }

  if (gasAlert) {
    if (now - lastGasBlinkMs >= GAS_ALARM_BLINK_MS) {
      lastGasBlinkMs = now;
      gasBlinkState = !gasBlinkState;
      if (currentScreen == SCREEN_GAS || currentScreen == SCREEN_HOME) {
        displayDirty = true;
      }
    }
  } else {
    gasBlinkState = false;
  }

  if (displayDirty && (fireAlert || quakeAlert || gasAlert)) {
    if (currentScreen == SCREEN_HOME || currentScreen == SCREEN_GAS ||
        currentScreen == SCREEN_FIRE || currentScreen == SCREEN_QUAKE) {
      renderScreen();
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_BTN_PREV, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_FIRE_DO, INPUT_PULLUP);
  pinMode(PIN_QUAKE, INPUT);
  setBuzzer(false);

  Wire.begin();
  Wire.setClock(100000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("ERRO: OLED nao encontrado"));
    for (;;) delay(1000);
  }

  dhtSensor.begin();
  delay(2000);

  Serial.println(F("===== SMART HOME LAB ====="));
  Serial.println(F("NEXT 3 | PREV 4 | Buzzer 6 | fogo>sismo>gas"));
  mqReadyAtMs = millis() + MQ_WARMUP_MS;
  drawWarmupScreen();

  displayDirty = true;
}

void finishMqWarmup() {
  calibrateGasBaseline();
  Serial.print(F("Baseline gas A1="));
  Serial.println(gasBaseline);

  calibrateFireBaseline();
  Serial.print(F("Baseline fogo A2="));
  Serial.println(fireBaseline);

  calibrateQuakeIdle();
  Serial.print(F("KY-002 repouso A3="));
  Serial.println(quakeIdleLevel == HIGH ? F("HIGH") : F("LOW"));

  mqReady = true;
  renderScreen();
}

void loop() {
  handleButtons();

  if (!mqReady) {
    if (millis() >= mqReadyAtMs) {
      finishMqWarmup();
    } else {
      if (currentScreen == SCREEN_HOME) {
        static unsigned long lastWarmupDrawMs = 0;
        if (millis() - lastWarmupDrawMs >= 1000) {
          lastWarmupDrawMs = millis();
          drawWarmupScreen();
        }
      } else {
        renderScreenIfNeeded(false);
      }
      delay(10);
      return;
    }
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

  if (millis() - lastFireMs >= READ_FIRE_MS) {
    lastFireMs = millis();
    bool prevAlert = fireAlert;
    int prevPercent = firePercent;
    updateFire();
    emitTelemetryJson();

    if (fireAlert != prevAlert || abs(firePercent - prevPercent) >= 2 ||
        firePercent != lastDrawnFirePercent) {
      displayDirty = true;
    }
  }

  if (millis() - lastQuakeMs >= READ_QUAKE_MS) {
    lastQuakeMs = millis();
    bool prevAlert = quakeAlert;
    int prevPercent = quakePercent;
    updateQuake();
    emitTelemetryJson();

    if (quakeAlert != prevAlert || quakePercent != prevPercent ||
        quakePercent != lastDrawnQuakePercent) {
      displayDirty = true;
    }
  }

  renderScreenIfNeeded(false);
  updateAlarms();
  delay(10);
}
