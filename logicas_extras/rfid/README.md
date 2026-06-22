# RFID â€” PrÃ©dio Inteligente

Acesso por cartÃ£o RFID â€” porta do prÃ©dio e apartamentos por andar.

## Hardware comprado

| Componente | CÃ³digo Mauser | Fatura |
|------------|---------------|--------|
| Leitor RFID RC522 13,56 MHz | 096-8517 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| NodeMCU ESP32 USB-C (plataforma) | 095-4900 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |

## Estrutura

| Pasta | ConteÃºdo |
|-------|----------|
| [simulacao/](simulacao/) | SimulaÃ§Ã£o / mock de cartÃµes |
| [real/](real/) | Firmware RC522 + ESP32 |
| [testes/](testes/) | Leitura UID, autorizaÃ§Ã£o |
| [documentacao/](documentacao/) | Mapa de cartÃµes / utilizadores |

## Estado

**Inicial** â€” leitor comprado; lÃ³gica de autorizaÃ§Ã£o por definir.

## Etapa

[E12 â€” LÃ³gicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md)
