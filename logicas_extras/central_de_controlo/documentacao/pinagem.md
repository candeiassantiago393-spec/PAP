# Pinagem — Central de Controlo (ESP32)

| Função | GPIO | Tipo | Notas |
|--------|------|------|-------|
| OLED SDA | 21 | I2C | SSD1306 0x3C |
| OLED SCL | 22 | I2C | |
| Botão NEXT | 32 | INPUT_PULLUP | LOW = premido |
| Botão OK | 33 | INPUT_PULLUP | LOW = premido |
| PIR | 27 | INPUT | HIGH = movimento |
| Buzzer | 25 | OUTPUT | `tone()` / digital |
| DHT22 | 26 | INPUT | Temperatura + humidade |
| Solo | 34 | ADC1 | Analógico — potenciómetro na simulação |
| Gás | 35 | ADC1 | Analógico — potenciómetro na simulação |
| Fogo | 4 | ADC2 | Analógico — potenciómetro na simulação |
| Sismo | 13 | ADC2 | Analógico — potenciómetro na simulação |
| LED vermelho | 16 | OUTPUT | Alarme fogo / intruso |
| LED amarelo | 17 | OUTPUT | Alarme gás / solo molhado |
| LED verde | 18 | OUTPUT | Solo ideal / alarme sismo |
| Bomba | 19 | OUTPUT | LED na simulação Wokwi |

## Limites (firmware actual)

| Parâmetro | Valor |
|-----------|-------|
| `GAS_LIMIT` | 50 % |
| `FIRE_LIMIT` | 50 % |
| `QUAKE_LIMIT` | 50 % |
| `SOIL_LIMIT_LOW` | 30 % |
| `SOIL_LIMIT_HIGH` | 70 % |
| `PIR_COOLDOWN_MS` | 30 000 ms |

## Notas ADC

- GPIO 34, 35 = **ADC1** — funcionam sempre.
- GPIO 4, 13 = **ADC2** — não ler com Wi‑Fi activo. Este projecto não usa Wi‑Fi por agora.

## Alimentação

- OLED e sensores: 3V3 do ESP32
- PIR: 5V (conforme diagram.json Wokwi)
- Buzzer, LEDs, bomba: GPIO + GND (resistências 220 Ω nos LEDs)
