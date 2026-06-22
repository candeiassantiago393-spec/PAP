# PainÃ©is de Controlo â€” PrÃ©dio Inteligente

Interfaces de supervisÃ£o e comando geral do prÃ©dio.

## Objetivo

Painel central (ou distribuÃ­do) para visualizar estado dos subsistemas: elevador, seguranÃ§a, meteorologia, acessos.

## Hardware disponÃ­vel

| Componente | Uso |
|------------|-----|
| OLED 0,96" (096-7806) | Display protÃ³tipo |
| ESP32 (095-4900) | Controlador com Wi-Fi |
| App geral-controll-app | OrganizaÃ§Ã£o e documentaÃ§Ã£o externa |

## Estrutura

| Pasta | ConteÃºdo |
|-------|----------|
| [simulacao/](simulacao/) | Mock de interface |
| [real/](real/) | Firmware painel fÃ­sico |
| [testes/](testes/) | Testes de UI / botÃµes |
| [documentacao/](documentacao/) | Wireframes, estados |

## LigaÃ§Ã£o Ã  app

Interface web desenvolvida pelo autor: [interface/README.md](../../interface/README.md)

## Estado

**Inicial** â€” conceito; integraÃ§Ã£o com elevador e extras por planear.

## Etapa

[E12 â€” LÃ³gicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md)
