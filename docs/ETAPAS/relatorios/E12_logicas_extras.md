# E12 — Lógicas Extras do Prédio Inteligente

| Campo | Valor |
|-------|-------|
| **Estado** | Futuro (versões iniciais existem) |
| **Código** | `logicas_extras/` |

## Objetivo

Subsistemas do prédio além do elevador: segurança, RFID, meteorologia, porta, toldo, etc.

## Módulos planeados

| Módulo | Estado |
|--------|--------|
| **Cofre código secreto** | Simulação Wokwi — [logicas_extras/cofre/](../../logicas_extras/cofre/) |
| Segurança | Inicial |
| Painéis controlo | Inicial |
| Meteorologia | Inicial |
| Porta prédio + código | Inicial (relacionado com cofre) |
| RFID por andar | Inicial |
| Toldo automático (chuva) | Inicial |

## Prioridade

Após conclusão do elevador (E10).

## O que foi feito

### Cofre — código secreto (Wokwi)

- Protótipo funcional: teclado 3×4, validação PIN, servo, LEDs e buzzer
- Código correcto de demonstração: `1904`
- Export integrado em `logicas_extras/cofre/simulacao/wokwi/`
- Link: https://wokwi.com/projects/462220624733136897

Relacionado com a porta do prédio (mesma lógica de acesso por código).

_(Outros módulos a documentar à medida que evoluem.)_

## Compras relacionadas

| Fatura | Total | Componentes |
|--------|-------|-------------|
| [2026EC1403323](../compras/faturas_mauser/2026EC1403323.md) | 29,62 € | ESP32, teclado, RFID, OLED protótipo |
| [2026EC1409725](../compras/faturas_mauser/2026EC1409725.md) | 47,18 € | T-Camera S3 (câmara + PIR) |

Ver [índice Mauser](../compras/faturas_mauser/_indice.md).
