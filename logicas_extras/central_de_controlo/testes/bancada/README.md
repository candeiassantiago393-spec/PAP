# Testes de Bancada — Central de Controlo (Arduino Mega)

Validação de hardware real na bancada com **Arduino Mega 2560** — mesma metodologia do elevador. O firmware completo em `../../` compila também para Mega (`pio run -e megaatmega2560`).

## Estrutura (PlatformIO)

Cada pasta tem `src/main.ino` + `platformio.ini` + `README.md`.

| Pasta | Teste | Serial |
|-------|-------|--------|
| [dht_oled/](dht_oled/) | KY-015 + OLED SSD1306 — HOME, ambiente, gráficos temp/hum, QR | 9600 |
| [dht_gas_oled_next_buzzer/](dht_gas_oled_next_buzzer/) | KY-015 + MQ + OLED + NEXT + buzzer — junção combinada | 9600 |
| [gas_oled/](gas_oled/) | MQ gás + OLED — HOME, barra gás, gráfico, alarme (NEXT/PREV) | 9600 |
| [fire_oled/](fire_oled/) | KY-026 + OLED — HOME, barra fogo, gráfico, alarme | 9600 |
| [fire_serial/](fire_serial/) | KY-026 — **só Serial Monitor** (diagnóstico) | 9600 |
| [pir_serial/](pir_serial/) | PIR HC-SR501 — **só Serial Monitor** (movimento) | 9600 |

## Upload rápido

```powershell
cd logicas_extras/central_de_controlo/testes/bancada/dht_oled
pio run -t upload --upload-port COMx
pio device monitor
```

## Ordem recomendada na bancada

1. `dht_oled` — I2C OLED + leituras KY-015 (Mega)
2. `gas_oled` — MQ AO=A1 + OLED + botões NEXT/PREV (Mega)
3. `fire_oled` — KY-026 AO=A2 + OLED + botão NEXT (Mega)
4. Firmware completo — `../../` Mega ou ESP32

Ver [REGISTO_2026-07-11.md](REGISTO_2026-07-11.md) (primeira validação) e [REGISTO_2026-07-12.md](REGISTO_2026-07-12.md) (soldadura, cablagem, gráficos temp/hum separados).

## Referência simulação

Wokwi (ESP32): [../../diagram.json](../../diagram.json)
