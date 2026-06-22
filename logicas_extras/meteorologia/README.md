# Meteorologia — Prédio Inteligente

Estação meteorológica por **andar** do prédio: temperatura e humidade no hall / zona comum, visíveis num **ecrã OLED** (projeto **ecra** / painel de controlo).

## Visão na maquete

```text
Prédio PAP (objectivo)
├── Andar 1 → ecra OLED + DHT22
├── Andar 2 → ecra OLED + DHT22
├── Andar 3 → ecra OLED + DHT22
└── Andar 4 → ecra OLED + DHT22

Demonstração actual (E12)
└── Uma caixa ESP32 — ecrãs AMBIENTE + GRÁFICO no painel ecra
```

O módulo **meteorologia** documenta a *lógica* e o *hardware* ambiental. O **código e simulação** vivem em [paineis_controlo/](../paineis_controlo/) (migrado do projeto ecra).

## Protótipo funcional (painel ecra)

| Ecrã OLED | Conteúdo | Firmware |
|-----------|----------|----------|
| **AMBIENTE** | Temperatura e humidade (DHT22) | `SCREEN_ENV` |
| **GRÁFICO** | Sparkline das últimas leituras de temperatura | `SCREEN_GRAPH` |

| Onde correr | Pasta |
|-------------|-------|
| Wokwi | [paineis_controlo/simulacao/wokwi/](../paineis_controlo/simulacao/wokwi/) |
| PlatformIO | [paineis_controlo/real/caixa_ecra/](../paineis_controlo/real/caixa_ecra/) |

```bash
cd logicas_extras/paineis_controlo/real/caixa_ecra && pio run
```

## Hardware (demo actual)

| Componente | Ligação | Notas |
|------------|---------|-------|
| DHT22 | GPIO **26** (ESP32) | Temp. + humidade relativa |
| OLED SSD1306 | I2C GPIO 21/22 | 128×64, addr 0x3C |
| ESP32 NodeMCU | USB-C | [095-4900](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| OLED protótipo | — | [096-7806](../../docs/compras/faturas_mauser/2026EC1403323.md) |

Leitura DHT: a cada **2 s** · histórico gráfico: **48 amostras** (~96 s de temperatura).

## Estrutura deste módulo

| Pasta | Conteúdo |
|-------|----------|
| [documentacao/](documentacao/) | [Funcionamento](documentacao/funcionamento.md), [pinagem](documentacao/pinagem.md) |
| [simulacao/](simulacao/) | → painel ecra Wokwi |
| [real/](real/) | → caixa_ecra PlatformIO |
| [testes/](testes/) | Checklist DHT + ecrãs |

## Estado

| Fase | Estado |
|------|--------|
| Lógica DHT22 + ecrãs AMBIENTE/GRÁFICO | **Funcional** (painel ecra) |
| Um painel na maquete (demo) | Em curso |
| Um ecra por andar | **Planeado** (replicar hardware/firmware) |

## Documentação técnica

- [funcionamento.md](documentacao/funcionamento.md) — leituras, ecrãs, gráfico
- [pinagem.md](documentacao/pinagem.md) — DHT22 e OLED (ESP32)
- Fonte de verdade do firmware: [paineis_controlo/documentacao/](../paineis_controlo/documentacao/)

## Etapa

[E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md)

## Navegação

| | |
|--|--|
| Painel ecra (código) | [../paineis_controlo/](../paineis_controlo/) |
| Segurança (mesmo painel) | [../seguranca/](../seguranca/) |
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
