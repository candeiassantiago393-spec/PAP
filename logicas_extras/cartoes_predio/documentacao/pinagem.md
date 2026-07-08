# Pinagem — RC522 + ESP32 (planeado)

Ligação típica do módulo **RC522** ao **ESP32** via SPI.

| RC522 | ESP32 | Notas |
|-------|-------|-------|
| SDA (SS) | GPIO 5 | Chip select |
| SCK | GPIO 18 | SPI clock |
| MOSI | GPIO 23 | SPI data out |
| MISO | GPIO 19 | SPI data in |
| IRQ | — | Opcional (não usado na v1) |
| GND | GND | Massa comum |
| RST | GPIO 22 | Reset do módulo |
| 3.3V | 3V3 | **Nunca 5 V** no RC522 |

## Actuador (fechadura)

| Função | Pino | Notas |
|--------|------|-------|
| Relé / servo | A definir | Módulo relé do kit Mauser ou servo partilhado com codigo_predio |

> **Nota:** Confirmar pinagem na montagem real — evitar conflito com OLED/I2C da central de controlo se partilharem o mesmo ESP32.

## Biblioteca Arduino

- `MFRC522` (PlatformIO: `mobizt/MFRC522`)
