# RFID — Prédio Inteligente

Acesso por cartão RFID — porta do prédio e apartamentos por andar.

## Hardware comprado

| Componente | Código Mauser | Fatura |
|------------|---------------|--------|
| Leitor RFID RC522 13,56 MHz | 096-8517 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| NodeMCU ESP32 USB-C (plataforma) | 095-4900 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/](simulacao/) | Simulação / mock de cartões |
| [real/](real/) | Firmware RC522 + ESP32 |
| [testes/](testes/) | Leitura UID, autorização |
| [documentacao/](documentacao/) | Mapa de cartões / utilizadores |

## Relacionado

| Módulo | Ligação |
|--------|---------|
| Porta do prédio | [porta_predio/](../porta_predio/) |
| Painel ecra | [paineis_controlo/](../paineis_controlo/) |

## Estado

**Inicial** — leitor comprado; lógica de autorização por definir.

## Navegação

| | |
|--|--|
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
| Etapa | [E12](../../docs/ETAPAS/relatorios/E12_logicas_extras.md) |
