# Meteorologia — Prédio Inteligente

Sensores ambientais do prédio (temperatura, humidade).

## Protótipo funcional

Implementado no painel ESP32 multi-ecrã:

- Ecrã **AMBIENTE** — DHT22
- Ecrã **GRÁFICO** — sparkline de temperatura

Ver [paineis_controlo/simulacao/wokwi/](../paineis_controlo/simulacao/wokwi/).

## Hardware

| Componente | Estado |
|------------|--------|
| DHT22 | Na simulação Wokwi (painel ecra) |
| ESP32 | [NodeMCU 095-4900](../../docs/compras/faturas_mauser/2026EC1403323.md) |

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/](simulacao/) | _(futuro: módulo dedicado)_ |
| [real/](real/) | Integração física pendente |
| [documentacao/](documentacao/) | Especificação |
| [testes/](testes/) | Calibração |

## Etapa

[E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md)

## Navegação

| | |
|--|--|
| Painel ecra | [../paineis_controlo/](../paineis_controlo/) |
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
