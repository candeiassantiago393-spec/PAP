# Segurança — Prédio Inteligente

Alarmes e deteção de intrusão no prédio.

## Protótipo funcional

Implementado no painel ESP32:

- Ecrã **MOVIMENTO** — sensor PIR, buzzer, cooldown 30 s
- Alertas **Telegram** opcionais (intruso)

Ver [paineis_controlo/documentacao/funcionamento.md](../paineis_controlo/documentacao/funcionamento.md).

## Hardware planeado

| Componente | Estado |
|------------|--------|
| PIR | Simulação Wokwi (painel ecra) |
| T-Camera S3 | [2026EC1409725](../../docs/compras/faturas_mauser/2026EC1409725.md) — futuro snapshot |

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/](simulacao/) | _(futuro)_ |
| [real/](real/) | Montagem física |
| [documentacao/](documentacao/) | Estados de alarme |
| [testes/](testes/) | Testes PIR / sirene |

## Etapa

[E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md)

## Navegação

| | |
|--|--|
| Painel ecra | [../paineis_controlo/](../paineis_controlo/) |
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
