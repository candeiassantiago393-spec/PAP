# Porta do Prédio — Código de Acesso

Entrada ao prédio por código numérico no teclado matricial.

## Hardware comprado

| Componente | Código Mauser | Fatura |
|------------|---------------|--------|
| Teclado matricial 3×4 | 096-4642 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| NodeMCU ESP32 USB-C | 095-4900 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| OLED 0,96" protótipo | 096-7806 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |

## Relação com outros módulos

- **[Cofre código secreto](../cofre/)** — protótipo Wokwi **já funcional** com a mesma lógica de PIN; base para evoluir esta porta
- Teclado matricial 3×4 comprado (Mauser) partilhado entre cofre e porta

## Funcionalidade prevista

- Utilizador introduz código no teclado
- Sistema valida combinação
- Aciona fechadura / sinal de porta aberta
- Feedback no OLED (mensagens de erro/sucesso)

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/](simulacao/) | Teste de lógica de código |
| [real/](real/) | Firmware teclado + ESP32 |
| [testes/](testes/) | Debounce teclas, validação PIN |
| [documentacao/](documentacao/) | Códigos de teste, pinagem |

## Estado

**Inicial** — componentes comprados; firmware por desenvolver. Ver protótipo em [cofre/simulacao/wokwi/](../cofre/simulacao/wokwi/).

## Etapa

[E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md)
