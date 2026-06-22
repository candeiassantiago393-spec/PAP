# Painéis de Controlo — Prédio Inteligente

Interface **ESP32 + OLED** multi-ecrã — agrega meteorologia, segurança e estufa numa caixa de demonstração (projeto **ecra** / candeias.dev).

## Estado

**Simulação Wokwi + PlatformIO** — migrado para o repo PAP (junho 2026).

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/wokwi/](simulacao/wokwi/) | `diagram.json`, `sketch.ino`, Wokwi |
| [real/caixa_ecra/](real/caixa_ecra/) | PlatformIO ESP32, Telegram, `secrets.example.h` |
| [documentacao/](documentacao/) | [Funcionamento](documentacao/funcionamento.md), [pinagem](documentacao/pinagem.md) |
| [testes/](testes/) | Checklist simulação |
| [caixa_ecra_esp32_oled.ino](caixa_ecra_esp32_oled.ino) | Cópia legada do sketch |

## Subsistemas cobertos no painel

| Ecrã | Ligação lógicas extras |
|------|------------------------|
| AMBIENTE / GRÁFICO | [meteorologia/](../meteorologia/) |
| MOVIMENTO | [seguranca/](../seguranca/) |
| PLANTAS | Estufa (referência Projeto-Estufa) |
| HOME / QR | Portfolio [candeias.dev](https://candeias.dev) |

## Simular no Cursor

```bash
cd logicas_extras/paineis_controlo/real/caixa_ecra
pio run
```

Abrir `simulacao/wokwi/` → `F1` → **Wokwi: Start Simulator**

## Hardware

| Componente | Nota |
|------------|------|
| ESP32 NodeMCU | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| OLED 0,96" SSD1306 | Protótipo painel |
| DHT22, PIR, botões | Ver [diagram.json](simulacao/wokwi/diagram.json) |

## App de organização

[interface/README.md](../../interface/README.md) — geral-controll-app

## Etapa

[E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md)

## Origem

Projeto local `ecra` (caixa inteligente MagSafe-style) — integrado no PAP para apresentação 17 julho.

## Navegação

| | |
|--|--|
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
| Etapa E12 | [../../docs/ETAPAS/relatorios/E12_logicas_extras.md](../../docs/ETAPAS/relatorios/E12_logicas_extras.md) |
