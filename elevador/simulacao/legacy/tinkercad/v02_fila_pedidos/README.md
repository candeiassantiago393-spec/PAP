# Circuito v02 — Tinkercad com fila de pedidos

Esta versão corresponde à segunda fase de desenvolvimento do projeto do elevador de 4 pisos em Tinkercad.

O circuito mantém a base da primeira versão, utilizando Arduino Uno, motor DC, ponte H, LCD I2C, botões exteriores, LEDs, buzzer e sensores simulados por interruptores. A principal evolução desta fase ocorreu no código, com a introdução de uma lógica de pedidos pendentes.

## Objetivo desta versão

O objetivo principal desta versão foi melhorar a lógica de funcionamento do elevador, permitindo memorizar vários pedidos de andares em simultâneo.

Na versão anterior, o sistema trabalhava essencialmente com uma variável de destino, o que fazia com que o último botão pressionado substituísse o destino anterior. Nesta versão foi introduzido um array de pedidos, permitindo que cada andar tivesse o seu próprio estado de pedido ativo.

## Principal alteração implementada

Foi introduzido o array:

```cpp
bool pedidos[4] = { false, false, false, false };
```

---

## Feedback dos orientadores

- **Manuel Queiroz** — identificou falta de paragem correcta por piso na v01
- **Rui Barros** — sugeriu utilização de **arrays** para gerir pedidos

## Hardware

Circuito **idêntico à v01**. Imagens do Tinkercad: [v01/imagens](../v01_uno_motor_dc_lcd/imagens/)

## Etapa do projeto

[E02 — Tinkercad v02](../../../../docs/ETAPAS/relatorios/E02_tinkercad_v02.md)

## Documentação

- [relatorio_melhoria_codigo.pdf](documentacao/relatorio_melhoria_codigo.pdf)