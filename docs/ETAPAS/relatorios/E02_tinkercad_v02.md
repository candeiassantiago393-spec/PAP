# E02 — Tinkercad v02 — Fila de Pedidos

| Campo | Valor |
|-------|-------|
| **Estado** | Concluída |
| **Período** | Após setembro 2025 _(sequência directa à v01)_ |
| **Código** | [elevador/simulacao/legacy/tinkercad/v02_fila_pedidos](../../../elevador/simulacao/legacy/tinkercad/v02_fila_pedidos/) |
| **Hardware** | **Igual à v01** — só mudou o código |

## Objetivo

Memorizar vários pedidos de pisos em simultâneo e corrigir a paragem por piso.

## Contexto pedagógico

Durante a revisão do projeto, o orientador **Manuel Queiroz** identificou que **não existia paragem correcta por piso** — o elevador não se comportava como devia ao atender cada andar.

O orientador **Rui Barros** sugeriu a utilização de **arrays** para gerir o estado dos pedidos.

## O que foi feito

- Introdução do array `bool pedidos[4]` — um estado por piso
- Correcção da lógica de paragem em cada andar pedido
- Circuito Tinkercad **inalterado** face à v01 (mesmo Uno, motor DC, LCD, botões, LEDs)
- Múltiplos pedidos memorizados em simultâneo

## Decisões técnicas

- Evolução **só em software** antes de mudar plataforma ou hardware
- Arrays como estrutura base para a fila — padrão mantido até v06

## Dificuldades

- Nenhuma grave após feedback dos orientadores — **funcionou logo** com a nova lógica

## Resultado / validação

- Vários botões pressionados ficam registados
- LEDs indicam pedidos activos
- Paragem por piso corrigida

## Limitações que levaram à v03

- Arduino Uno sem pinos para expansão (interiores, porta, reset…)
- Tinkercad limitado para Mega → transição Wokwi

## Evidências

| Ficheiro | Descrição |
|----------|-----------|
| [v02 README](../../../elevador/simulacao/legacy/tinkercad/v02_fila_pedidos/README.md) | Documentação da versão |
| [relatorio_melhoria_codigo.pdf](../../../elevador/simulacao/legacy/tinkercad/v02_fila_pedidos/documentacao/relatorio_melhoria_codigo.pdf) | Relatório de melhoria do código |
| Circuito v01 | [imagens da v01](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/imagens/) _(hardware idêntico)_ |

## Próximo passo

→ [E03 — Wokwi + Mega + A4988](E03_wokwi_v03.md)
