# Toldo Automático — Prédio Inteligente

Estendal que se fecha automaticamente quando chove.

## Objetivo

Sensor de chuva deteta precipitação → aciona motor/servo do toldo para proteger varanda/zona exterior da maquete.

## Hardware

| Componente | Estado |
|------------|--------|
| Sensor de chuva | Por adquirir |
| Actuador (servo/relé) | Por definir |
| Microcontrolador | ESP32 ou Mega (partilhado) |

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/](simulacao/) | Lógica chuva → acção |
| [real/](real/) | Firmware + sensor |
| [testes/](testes/) | Teste sensor molhado/seco |
| [documentacao/](documentacao/) | Esquema de ligação |

## Relacionado

| Módulo | Ligação |
|--------|---------|
| Meteorologia | [meteorologia/](../meteorologia/) — sensores ambientais no painel ecra |

## Estado

**Planeado** — conceito definido; componentes por adquirir.

## Navegação

| | |
|--|--|
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
| Etapa | [E12](../../docs/ETAPAS/relatorios/E12_logicas_extras.md) |
