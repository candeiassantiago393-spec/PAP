/*
  Test_01_External_Requests_LED_Latched.ino

  Objective:
  Test the external elevator request buttons and LEDs.

  Behaviour:
  - Press external button floor 1 -> LED floor 1 stays ON
  - Press external button floor 2 -> LED floor 2 stays ON
  - Press external button floor 3 -> LED floor 3 stays ON
  - Press external button floor 4 -> LED floor 4 stays ON

  This allows testing multiple requests at the same time.

  Hardware:
  - Arduino Mega
  - Buttons connected between Arduino input pin and GND
  - LEDs connected from Arduino output pin to resistor to LED to GND

  Logic:
  - INPUT_PULLUP is used
  - Button released = HIGH
  - Button pressed  = LOW
*/

// ================= EXTERNAL BUTTON PINOUT =================

// External button for floor 1 connected to digital pin 2
#define pinButtonExt1 2

// External button for floor 2 connected to digital pin 3
#define pinButtonExt2 3

// External button for floor 3 connected to digital pin 4
#define pinButtonExt3 4

// External button for floor 4 connected to digital pin 5
#define pinButtonExt4 5


// ================= EXTERNAL LED PINOUT =================

// External LED for floor 1 connected to analog pin A0 used as digital output
#define pinLedExt1 A0

// External LED for floor 2 connected to analog pin A1 used as digital output
#define pinLedExt2 A1

// External LED for floor 3 connected to analog pin A2 used as digital output
#define pinLedExt3 A2

// External LED for floor 4 connected to analog pin A3 used as digital output
#define pinLedExt4 A3


// ================= REQUEST MEMORY VARIABLES =================

// Stores the request state for floor 1
bool requestFloor1 = false;

// Stores the request state for floor 2
bool requestFloor2 = false;

// Stores the request state for floor 3
bool requestFloor3 = false;

// Stores the request state for floor 4
bool requestFloor4 = false;


void setup() {
  // Starts serial communication
  Serial.begin(9600);

  // Configures external buttons as inputs with internal pull-up resistors
  pinMode(pinButtonExt1, INPUT_PULLUP);
  pinMode(pinButtonExt2, INPUT_PULLUP);
  pinMode(pinButtonExt3, INPUT_PULLUP);
  pinMode(pinButtonExt4, INPUT_PULLUP);

  // Configures external LEDs as outputs
  pinMode(pinLedExt1, OUTPUT);
  pinMode(pinLedExt2, OUTPUT);
  pinMode(pinLedExt3, OUTPUT);
  pinMode(pinLedExt4, OUTPUT);

  // Makes sure all LEDs start OFF
  digitalWrite(pinLedExt1, LOW);
  digitalWrite(pinLedExt2, LOW);
  digitalWrite(pinLedExt3, LOW);
  digitalWrite(pinLedExt4, LOW);

  // Initial message
  Serial.println("Latched external request test started.");
}


void loop() {
  // Reads external button floor 1
  bool stateButtonExt1 = digitalRead(pinButtonExt1);

  // Reads external button floor 2
  bool stateButtonExt2 = digitalRead(pinButtonExt2);

  // Reads external button floor 3
  bool stateButtonExt3 = digitalRead(pinButtonExt3);

  // Reads external button floor 4
  bool stateButtonExt4 = digitalRead(pinButtonExt4);


  // If button floor 1 is pressed, store the request
  if (stateButtonExt1 == LOW) {
    requestFloor1 = true;
    Serial.println("External request floor 1 stored.");
  }

  // If button floor 2 is pressed, store the request
  if (stateButtonExt2 == LOW) {
    requestFloor2 = true;
    Serial.println("External request floor 2 stored.");
  }

  // If button floor 3 is pressed, store the request
  if (stateButtonExt3 == LOW) {
    requestFloor3 = true;
    Serial.println("External request floor 3 stored.");
  }

  // If button floor 4 is pressed, store the request
  if (stateButtonExt4 == LOW) {
    requestFloor4 = true;
    Serial.println("External request floor 4 stored.");
  }


  // LED floor 1 follows the stored request state
  digitalWrite(pinLedExt1, requestFloor1 ? HIGH : LOW);

  // LED floor 2 follows the stored request state
  digitalWrite(pinLedExt2, requestFloor2 ? HIGH : LOW);

  // LED floor 3 follows the stored request state
  digitalWrite(pinLedExt3, requestFloor3 ? HIGH : LOW);

  // LED floor 4 follows the stored request state
  digitalWrite(pinLedExt4, requestFloor4 ? HIGH : LOW);


  // Small delay to reduce repeated Serial Monitor messages
  delay(100);
}
