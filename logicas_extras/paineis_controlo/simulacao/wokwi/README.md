# Wokwi — Caixa ESP32 + OLED (painel prédio)

Simulação do **painel de controlo** multi-ecrã — ambiente, segurança, estufa, gráficos e QR portfolio.

Abrir **esta pasta** como raiz do projeto Wokwi no Cursor.

| Ficheiro | Uso |
|----------|-----|
| [sketch.ino](sketch.ino) | Firmware (espelho de `real/caixa_ecra/src/main.cpp`) |
| [diagram.json](diagram.json) | Circuito virtual ESP32 |
| [wokwi.toml](wokwi.toml) | Aponta para firmware compilado (PlatformIO) |
| [libraries.txt](libraries.txt) | Adafruit GFX/SSD1306, DHT |

## Antes de simular

O `wokwi.toml` usa o binário PlatformIO. Compilar primeiro:

```bash
cd ../../real/caixa_ecra
pio run
```

Depois: `F1` → **Wokwi: Start Simulator** (nesta pasta).

Após alterar código: **Stop + Start** na simulação.

## Ecrãs (6)

HOME · AMBIENTE (DHT22) · MOVIMENTO (PIR) · PLANTAS (solo) · GRÁFICO temp · Instagram QR

## Etapa

[E12 — Lógicas extras](../../../../docs/ETAPAS/relatorios/E12_logicas_extras.md)
