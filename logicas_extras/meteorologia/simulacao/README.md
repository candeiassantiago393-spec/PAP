# Simulação — Meteorologia

A simulação da meteorologia está no **painel ecra** (projeto ecra migrado para o PAP).

## Onde simular

| Ambiente | Pasta |
|----------|-------|
| **Wokwi** | [paineis_controlo/simulacao/wokwi/](../../paineis_controlo/simulacao/wokwi/) |

### Passos

1. Abrir `logicas_extras/paineis_controlo/simulacao/wokwi/`
2. `F1` → **Wokwi: Start Simulator**
3. Carregar **NEXT** até o ecrã **AMBIENTE** (DHT22) ou **GRÁFICO** (sparkline)

## Componentes na simulação

| ID Wokwi | Função |
|----------|--------|
| `dht1` | DHT22 — temperatura e humidade |
| `oled1` | SSD1306 128×64 |
| `esp` | ESP32 DevKit |

Ver [diagram.json](../../paineis_controlo/simulacao/wokwi/diagram.json).

## Firmware espelho

[sketch.ino](../../paineis_controlo/simulacao/wokwi/sketch.ino) — cópia de `real/caixa_ecra/src/main.cpp`.

## Navegação

| | |
|--|--|
| Módulo meteorologia | [../README.md](../README.md) |
| Funcionamento | [../documentacao/funcionamento.md](../documentacao/funcionamento.md) |
