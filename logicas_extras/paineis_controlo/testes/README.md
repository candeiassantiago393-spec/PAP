# Testes — Painéis de controlo

## Simulação Wokwi

1. Compilar `real/caixa_ecra` com `pio run`
2. Abrir `simulacao/wokwi/` → Start Simulator
3. Serial Monitor 115200 baud

## Checklist manual

- [ ] NEXT percorre os 6 ecrãs
- [ ] DHT22 mostra temp/hum no ecrã AMBIENTE
- [ ] PIR activa alerta + buzzer; OK silencia
- [ ] Potenciómetro solo altera barra e LEDs (SECO/IDEAL/MOLHADO)
- [ ] Modo feira roda ecrãs automaticamente
- [ ] Telegram (opcional) — ver TELEGRAM.md
