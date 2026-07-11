#pragma once

// Pinagem por plataforma — central_de_controlo

#if defined(ARDUINO_AVR_MEGA2560)

#define ADC_MAX 1023
#define SERIAL_BAUD 115200

#define PIN_BTN_NEXT 3
#define PIN_BTN_OK 4
#define PIN_PIR 5
#define PIN_BUZZER 6
#define PIN_DHT 2  // KY-015 (S) — pull-up integrado no modulo

#define PIN_SOIL A0
#define PIN_GAS A1
#define PIN_FIRE A2
#define PIN_QUAKE A3

#define PIN_LED_RED 7
#define PIN_LED_YELLOW 8
#define PIN_LED_GREEN 9
#define PIN_PUMP 10

// Mega: I2C OLED em SDA=20, SCL=21 (Wire default)

#else  // ESP32

#define ADC_MAX 4095
#define SERIAL_BAUD 115200

#define PIN_BTN_NEXT 32
#define PIN_BTN_OK 33
#define PIN_PIR 27
#define PIN_BUZZER 25
#define PIN_DHT 26  // KY-015 (S) — pull-up integrado no modulo

#define PIN_SOIL 34
#define PIN_LED_RED 16
#define PIN_LED_YELLOW 17
#define PIN_LED_GREEN 18
#define PIN_PUMP 19

#define PIN_GAS 35
#define PIN_FIRE 4
#define PIN_QUAKE 13

// ESP32: I2C OLED em SDA=21, SCL=22 (Wire default)

#endif
