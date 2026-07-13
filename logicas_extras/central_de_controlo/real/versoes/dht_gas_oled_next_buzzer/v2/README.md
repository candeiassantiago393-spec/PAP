# v2 — dht + gas + oled + next + buzzer

| Campo | Valor |
|-------|--------|
| Data | 2026-07-13 |
| Plataforma | Arduino Mega 2560 |
| Origem | [../../../../testes/bancada/dht_gas_oled_next_buzzer/](../../../../testes/bancada/dht_gas_oled_next_buzzer/) |

## Novidades face a v1

- **Telemetria JSON** na Serial (`t`, `h`, `gas`, `alarm`, `scr`) — liga ao [dashboard](../../../../dashboard/)
- **Refresh OLED** com `displayDirty` + limite 1 s (menos flicker)
- **Ecrã warmup** MQ durante os 30 s iniciais
- **Cooldown** no botão NEXT (anti-rebote melhorado)
- **Alarme buzzer** contínuo enquanto gás ≥ 50 %

## Upload

```powershell
cd logicas_extras/central_de_controlo/real/versoes/dht_gas_oled_next_buzzer/v2
pio run -t upload --upload-port COMx
```

Ou via dashboard: `.\run_dashboard.cmd -Port COM7` (na raiz do repo).
