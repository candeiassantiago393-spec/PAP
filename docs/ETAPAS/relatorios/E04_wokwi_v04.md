# E04 — Wokwi v04 — 7 Segmentos + Reset/Rearme

| Campo | Valor |
|-------|-------|
| **Estado** | Concluída |
| **Período** | Dezembro 2025 |
| **Código** | [elevador/simulacao/legacy/wokwi/v04_7segmentos_reset_rearme](../../../elevador/simulacao/legacy/wokwi/v04_7segmentos_reset_rearme/) |
| **Wokwi** | https://wokwi.com/projects/454408518771138561 |

## Objetivo

Testar displays de 7 segmentos para indicação do piso e introduzir a **lógica de segurança** (reset, timeout, emergência e rearme).

## Por que displays de 7 segmentos

Na altura, os **displays de 7 segmentos** eram o recurso visual com que o autor já tinha experiência na escola. Os **OLEDs ainda não tinham sido utilizados** neste projecto.

Esta versão funcionou como **fase experimental**: validar indicação numérica do andar e, em paralelo, consolidar a lógica de segurança. Posteriormente, ao explorar o Wokwi e pesquisar alternativas, os **OLEDs** mostraram-se mais adequados — não só indicam o piso, mas permitem apresentar **fila de pedidos, estado do elevador, emergência, manutenção** e outra informação contextual. Essa evolução conduziu à v05.

## O que foi feito

- 4 displays de 7 segmentos multiplexados (`segmentPins[7]` em D22–D28)
- Botão de reset (D7)
- Estados: `ERRO_MOV`, `ERRO_PAR`, `REARME`, `ESTAB`
- Timeout de movimento + alarme contínuo no buzzer
- Debounce de botões
- LCD 20×4 I2C mantido

## Decisões técnicas

- Separar a experimentação visual (7 segmentos) da consolidação da segurança
- Multiplexagem para quatro dígitos com pinos dedicados
- Rearme com deslocação para posição segura antes de regressar ao normal

## Dificuldades

- Multiplexagem consome muitos pinos GPIO
- Informação limitada face a um display gráfico
- Lógica de emergência/rearme: **sem problemas relevantes** — implementação estável

## Resultado / validação

- Sistema entra e sai de emergência com reset
- Processo de rearme funcional
- Conclusão: 7 segmentos adequados para o piso actual, mas **insuficientes** para o painel informativo pretendido → transição para OLED na v05

## Evidências

| Ficheiro | Descrição |
|----------|-----------|
| [wokwi/](../../../elevador/simulacao/legacy/wokwi/v04_7segmentos_reset_rearme/wokwi/) | Export completo |
| [imagem_circuito.png](../../../elevador/simulacao/legacy/wokwi/v04_7segmentos_reset_rearme/imagem_circuito.png) | Vista do circuito |

## Próximo passo

→ [E05 — OLED via TCA9548A](E05_wokwi_v05.md)
