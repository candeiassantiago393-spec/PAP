# Teste — Integrado (Hall + OLED + pedidos exteriores)

Combina sensores Hall, 4 OLEDs via TCA9548A e botões/LEDs exteriores. **Sem motor, sem porta D22, sem botões interiores.**

## Pinagem

| Função | Pinos |
|--------|-------|
| Hall pisos 1–4 | D8, D11, D12, D13 (LOW = íman) |
| Botões exteriores | D2–D5 |
| LEDs exteriores | A0–A3 |
| I2C (TCA + OLED) | SDA D20, SCL D21 |

## Comportamento

- Se I2C falhar, **Hall + botões + LEDs continuam** (não bloqueia)
- Cada OLED mostra estado do respectivo piso
- Serial 9600 — estado a cada 500 ms

## Upload

```bash
cd elevador/testes/bancada/integrado
pio run -t upload --upload-port COM8
pio device monitor
```

## Etapa

[E07 — Testes de bancada](../../../../docs/ETAPAS/relatorios/E07_testes_bancada.md)
