# actual — versão real mais recente

Cópia de **dht_gas_oled_next_buzzer v1** (2026-07-13).

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
| Buzzer | 6 |
| MQ AOUT | A1 |
| OLED SDA / SCL | 20 / 21 |

## Histórico

| Data | Versão origem |
|------|---------------|
| 2026-07-13 | `versoes/dht_gas_oled_next_buzzer/v1` |
