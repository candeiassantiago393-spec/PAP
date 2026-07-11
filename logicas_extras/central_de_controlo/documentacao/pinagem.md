# Pinagem — Central de Controlo

O firmware (`src/main.cpp`) compila para **ESP32** ou **Arduino Mega 2560** — pinagem em [`pins.h`](../pins.h).

## ESP32 (caixa final / Wokwi)

| Função | GPIO | Tipo | Notas |
|--------|------|------|-------|
| OLED SDA | 21 | I2C | SSD1306 0x3C |
| OLED SCL | 22 | I2C | |
| Botão NEXT | 32 | INPUT_PULLUP | LOW = premido |
| Botão OK | 33 | INPUT_PULLUP | LOW = premido |
| PIR | 27 | INPUT | HIGH = movimento |
| Buzzer | 25 | OUTPUT | `tone()` / digital |
| KY-015 (DHT11) | 26 | INPUT | **(-)** GND, **(S)** GPIO 26, **(+)** 3V3 — pull-up no modulo |
| Solo | 34 | ADC1 | Analógico — potenciómetro na simulação |
| Gás | 35 | ADC1 | Analógico — potenciómetro na simulação |
| Fogo | 4 | ADC2 | Analógico — potenciómetro na simulação |
| Sismo | 13 | ADC2 | Analógico — potenciómetro na simulação |
| LED vermelho | 16 | OUTPUT | Alarme fogo / intruso |
| LED amarelo | 17 | OUTPUT | Alarme gás / solo molhado |
| LED verde | 18 | OUTPUT | Solo ideal / alarme sismo |
| Bomba | 19 | OUTPUT | LED na simulação Wokwi |

## Arduino Mega 2560 (bancada / elevador)

| Função | Pin | Tipo | Notas |
|--------|-----|------|-------|
| OLED SDA | 20 | I2C | SSD1306 0x3C |
| OLED SCL | 21 | I2C | |
| KY-015 (DHT11) | 2 | INPUT | **(-)** GND, **(S)** pin 2, **(+)** 5V |
| Botão NEXT | 3 | INPUT_PULLUP | LOW = premido |
| Botão OK | 4 | INPUT_PULLUP | LOW = premido |
| PIR | 5 | INPUT | HIGH = movimento |
| Buzzer | 6 | OUTPUT | |
| LED vermelho | 7 | OUTPUT | |
| LED amarelo | 8 | OUTPUT | |
| LED verde | 9 | OUTPUT | |
| Bomba | 10 | OUTPUT | |
| Solo | A0 | ADC | Potenciómetro / sensor capacitivo |
| Gás | A1 | ADC | MQ-2/MQ-135 |
| Fogo | A2 | ADC | KY-026 (AO) |
| Sismo | A3 | ADC | SW-420 (simulado / analógico) |

## Limites (firmware actual)

| Parâmetro | Valor |
|-----------|-------|
| `GAS_LIMIT` | 50 % |
| `FIRE_LIMIT` | 50 % |
| `QUAKE_LIMIT` | 50 % |
| `SOIL_LIMIT_LOW` | 30 % |
| `SOIL_LIMIT_HIGH` | 70 % |
| `PIR_COOLDOWN_MS` | 30 000 ms |

## Notas ADC (ESP32)

- GPIO 34, 35 = **ADC1** — funcionam sempre.
- GPIO 4, 13 = **ADC2** — não ler com Wi‑Fi activo. Este projecto não usa Wi‑Fi por agora.

## Alimentação (ESP32)

- OLED e sensores: 3V3 do ESP32
- PIR: 5V (conforme diagram.json Wokwi)
- Buzzer, LEDs, bomba: GPIO + GND (resistências 220 Ω nos LEDs)
