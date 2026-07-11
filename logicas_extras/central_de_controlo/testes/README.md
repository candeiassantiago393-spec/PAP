# Testes — Central de Controlo

Registo de testes na bancada e na caixa física.

## Código de teste (bancada)

Testes isolados em PlatformIO — ver [bancada/](bancada/):

| Pasta | Componentes | Liga a |
|-------|-------------|--------|
| [bancada/dht_oled/](bancada/dht_oled/) | KY-015 + OLED SSD1306 | T1, T2 |

Plataforma bancada: **Arduino Mega 2560** (SDA 20, SCL 21, KY-015 S pin 2).

```powershell
cd testes/bancada/dht_oled
pio run -t upload --upload-port COMx
pio device monitor   # 9600 baud
```

## Estado

**Em curso** — bancada Mega validada para T1 (OLED) e T2 (KY-015). Registo detalhado: [bancada/REGISTO_2026-07-11.md](bancada/REGISTO_2026-07-11.md).

## Testes planeados

| ID | Teste | Código | Resultado | Data |
|----|-------|--------|-----------|------|
| T1 | OLED + branding HOME | [bancada/dht_oled/](bancada/dht_oled/) | **OK** | 2026-07-11 |
| T2 | KY-015 leitura temp/hum | [bancada/dht_oled/](bancada/dht_oled/) + firmware Mega | **OK** | 2026-07-11 |
| T3 | PIR dispara alerta intruso | — | — | — |
| T4 | Sensor solo + bomba | — | — | — |
| T5 | MQ gás dispara FUGA DE GAS | — | — | — |
| T6 | KY-026 fogo dispara alarme | — | — | — |
| T7 | SW-420 sismo dispara alarme | — | — | — |
| T8 | Prioridade fogo &gt; sismo &gt; intruso &gt; gás | — | — | — |

## Referência simulação

Wokwi validado: [../diagram.json](../diagram.json) + `python -m platformio run`
