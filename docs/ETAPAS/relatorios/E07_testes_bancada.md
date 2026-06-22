# E07 — Testes de Bancada

| Campo | Valor |
|-------|-------|
| **Estado** | Concluída |
| **Período** | Fevereiro – Março 2026 |
| **Código** | [elevador/testes/bancada](../../../elevador/testes/bancada/) |

## Objetivo

Validar cada subsistema de hardware no Arduino Mega **antes** da montagem na maquete — primeiro isolado, depois integrado.

## Metodologia

1. **Fase isolada** — cada componente testado individualmente em breadboard + Mega (botões, LEDs, OLED, Hall, motor…)
2. **Fase integrada** — todos os módulos ligados em conjunto na bancada de testes

## Atraso — fonte de alimentação

O projecto **encerrou temporariamente** nesta fase porque a **fonte de alimentação industrial** (12 V / potência para motor + lógica) **demorou a chegar**. Os testes com carga real só puderam avançar após a recepção do componente.

> Ver também: [docs/compras/](../compras/) — faturas Mauser e justificação de compras _(a organizar)_.

## O que foi feito

| Teste | Ficheiro | Validação |
|-------|----------|-----------|
| Pedidos exteriores botão + LED | `pedidos_exteriores/Test_01_...` | Latch de pedido |
| Botões exteriores only | `pedidos_exteriores/Test_02_...` | INPUT_PULLUP |
| OLED bench | `oled/oledbecnch.ino` | SH1106 / SSD1306 |
| Sensor Hall | `sensores_hall/sensorhall.ino` | Deteção de piso |
| Integração completa | _(bancada fev–mar 2026)_ | Todos os módulos em conjunto |

## Decisões técnicas

- Pinagem igual à v06 / firmware real
- Validar hardware real antes da maquete
- Fonte dedicada para malha de potência (não depender só de USB)

## Dificuldades

### Atraso logístico — fonte de alimentação

Encomenda em atraso bloqueou testes com motor sob carga real até fevereiro/março.

### OLEDs + I2C a interferir com o motor

Os 4 displays no mesmo **SDA/SCL** causavam **gaguejar** do motor durante testes integrados.

**Solução:** refactor do código; versão actual do firmware real validada em bancada.

### Dimensionamento do motor

Contas de dimensionamento documentadas em [Dimensionamentos_motor.docx](../../relatorios/Dimensionamentos_motor.docx).

## Resultado / validação

- Componentes aprovados individualmente e em conjunto
- Bancada pronta para transição à maquete (E08)

## Evidências

| Ficheiro | Descrição |
|----------|-----------|
| [bancada_integracao_fev_mar_2026.png](../../../elevador/testes/bancada/imagens/bancada_integracao_fev_mar_2026.png) | Bancada completa — Mega, breadboards, OLEDs, motor, fonte |
| Testes `.ino` | [elevador/testes/bancada/](../../../elevador/testes/bancada/) |

## Compras relacionadas

| Componente | Fatura |
|------------|--------|
| Fonte 12 V Orno 5 A | [2026EC1454381](../../compras/faturas_mauser/2026EC1454381.md) — 7,72 € |
| OLED 1,3" ×4 | [2026EC1454381](../../compras/faturas_mauser/2026EC1454381.md) — 22,24 € |
| Kit resistências | [2026EC1454381](../../compras/faturas_mauser/2026EC1454381.md) — 5,24 € |
| Índice completo | [docs/compras/faturas_mauser/_indice.md](../../compras/faturas_mauser/_indice.md) |

## Próximo passo

→ [E08 — Montagem na maquete](E08_montagem_maquete.md)
