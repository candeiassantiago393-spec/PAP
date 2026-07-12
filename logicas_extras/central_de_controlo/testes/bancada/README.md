# Testes de Bancada — Central de Controlo (Arduino Mega)

Validação de hardware real na bancada com **Arduino Mega 2560** — mesma metodologia do elevador. O firmware completo em `../../` compila também para Mega (`pio run -e megaatmega2560`).

## Estrutura (PlatformIO)

Cada pasta tem `src/main.ino` + `platformio.ini` + `README.md`.

| Pasta | Teste | Serial |
|-------|-------|--------|
| [dht_oled/](dht_oled/) | KY-015 + OLED SSD1306 — HOME, ambiente, gráficos temp/hum, QR | 9600 |

## Upload rápido

```powershell
cd logicas_extras/central_de_controlo/testes/bancada/dht_oled
pio run -t upload --upload-port COMx
pio device monitor
```

## Ordem recomendada na bancada

1. `dht_oled` — I2C OLED + leituras KY-015 + ecrãs de demonstração (Mega)
2. Firmware completo — `../../` Mega (`pio run -e megaatmega2560`) ou ESP32 (`esp32dev`)

Ver [REGISTO_2026-07-11.md](REGISTO_2026-07-11.md) (primeira validação) e [REGISTO_2026-07-12.md](REGISTO_2026-07-12.md) (soldadura, cablagem, gráficos temp/hum separados).

## Referência simulação

Wokwi (ESP32): [../../diagram.json](../../diagram.json)
