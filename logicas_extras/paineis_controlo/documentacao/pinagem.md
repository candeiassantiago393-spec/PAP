# Pinagem — Caixa ESP32 OLED (painel)

> ESP32 DevKit · OLED SSD1306 I2C · Origem: projeto ecra / candeias.dev

| Função | GPIO | Notas |
|--------|------|-------|
| OLED SDA | 21 | I2C addr 0x3C |
| OLED SCL | 22 | |
| Botão NEXT | 32 | `INPUT_PULLUP`, LOW = premido |
| Botão OK | 33 | Silencia alarme PIR |
| PIR movimento | 27 | Cooldown 30 s |
| Buzzer | 25 | |
| DHT22 (temp/hum) | 26 | Ecrã AMBIENTE |
| Solo (analógico) | 34 | Potenciómetro sim / sensor real |
| LED vermelho | 16 | Solo SECO |
| LED amarelo | 17 | Solo IDEAL |
| LED verde | 18 | Solo MOLHADO |
| Bomba (sim LED) | 19 | Máx 10 s ON, pausa 30 s |

## Limites solo (estufa v1.3)

| Faixa | % | LED |
|-------|---|-----|
| SECO | &lt; 35 | Vermelho |
| IDEAL | 35–75 | Amarelo |
| MOLHADO | &gt; 75 | Verde |

## Simulação

[diagram.json](../simulacao/wokwi/diagram.json)
