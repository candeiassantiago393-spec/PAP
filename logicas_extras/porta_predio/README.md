# Porta do Prédio — Código de Acesso

Entrada ao prédio por código numérico no teclado matricial.

## Estado na maquete

| Aspeto | Decisão (jun 2026) |
|--------|---------------------|
| Montagem física | **Por decidir** — provável apresentação **só virtual** (Wokwi) |
| Código PIN demo | **`1904`** (igual ao cofre) |
| Localização lógica | **Rés-do-chão** = 1.º andar na maquete |

## Protótipo funcional (virtual)

Lógica já demonstrada no [cofre](../cofre/simulacao/wokwi/) — teclado 3×4, validação PIN, servo, LEDs, buzzer.

## Hardware comprado

| Componente | Código Mauser | Fatura |
|------------|---------------|--------|
| Teclado matricial 3×4 | 096-4642 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| NodeMCU ESP32 USB-C | 095-4900 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| OLED 0,96" protótipo | 096-7806 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |

## Relação com outros módulos

- **[Cofre](../cofre/)** — mesmo PIN `1904`, firmware Wokwi funcional
- Teclado matricial partilhado entre cofre e porta (quando montado)

## Funcionalidade prevista

- Código no teclado → validação → fechadura / feedback OLED
- Na apresentação: cofre Wokwi + narrativa da porta no rés-do-chão

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/](simulacao/) | Lógica PIN (herdar do cofre) |
| [real/](real/) | Firmware físico — pendente |
| [testes/](testes/) | Debounce, validação PIN |
| [documentacao/](documentacao/) | Códigos, pinagem |

## Etapa

[E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md)

## Navegação

| | |
|--|--|
| Cofre (protótipo PIN) | [../cofre/](../cofre/) |
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
