# actual — versão real mais recente

Cópia de **dht_gas_oled_next_buzzer v2** (2026-07-13), actualizada com **KY-026 fogo** (2026-07-15).

Quando disseres **«este é o real mais recente»**, esta pasta é actualizada a partir da versão indicada em `versoes/`.

## Upload

```powershell
cd logicas_extras/central_de_controlo/real/atual
pio run -t upload --upload-port COMx
pio device monitor -b 9600
```

## Pinagem Mega

| Função | Pin |
|--------|-----|
| KY-015 (S) | 2 |
| BTN NEXT | 3 |
| BTN PREV | 4 |
| KY-026 DO | 5 |
| Buzzer | 6 |
| MQ AOUT | A1 |
| KY-026 AO | A2 |
| KY-002 S (vibração) | A3 |
| OLED SDA / SCL | 20 / 21 |

## Telemetria JSON (dashboard)

Campos: `t`, `h`, `gas`, `gr`, `gb`, `alarm`, `fire`, `fr`, `fbl`, `fd`, `fa`, `qk`, `qr`, `qa`, `scr`, `ms`

Prioridade alarmes buzzer: **fogo/gás contínuo** · **sismo = 6 s uma única vez** desde cada upload.

## Histórico

| Data | Versão origem |
|------|---------------|
| 2026-07-15 | KY-026 fogo + ecrã FOGO + dashboard |
| 2026-07-13 | `versoes/dht_gas_oled_next_buzzer/v2` — telemetria JSON + dashboard |
| 2026-07-13 | `versoes/dht_gas_oled_next_buzzer/v1` — primeira junção combinada |
