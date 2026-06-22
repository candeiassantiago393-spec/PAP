# Firmware real — Caixa ESP32 OLED

Painel **candeias.dev** — ESP32 com OLED, botões, DHT22, PIR, estufa (solo/bomba) e alertas Telegram.

## Compilar

```bash
cd logicas_extras/paineis_controlo/real/caixa_ecra
pio run
pio run -t upload
pio device monitor
```

## Wokwi

Compilar aqui primeiro (`pio run`), depois abrir [simulacao/wokwi/](../../simulacao/wokwi/) → Start Simulator.

## Telegram / Wi-Fi

Ver [TELEGRAM.md](TELEGRAM.md). Copiar `secrets.example.h` → `secrets.h` (não versionar).

## Ficheiros

| Ficheiro | Uso |
|----------|-----|
| `src/main.cpp` | Fonte PlatformIO |
| `platformio.ini` | ESP32 dev board |
| `secrets.example.h` | Template credenciais |
| `gen_qr.py` | Regenerar bitmap QR Instagram |

## Origem

Migrado do projeto local **ecra** (junho 2026) para o repo PAP.
