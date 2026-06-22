# E03 — Wokwi v03 — Transição Mega + A4988

| Campo | Valor |
|-------|-------|
| **Estado** | Concluída |
| **Período** | Novembro 2025 |
| **Código** | [elevador/simulacao/legacy/wokwi/v03_transicao_mega_a4988](../../../elevador/simulacao/legacy/wokwi/v03_transicao_mega_a4988/) |
| **Wokwi** | https://wokwi.com/projects/454346932045509633 |

## Objetivo

Migrar para **Arduino Mega** e motor de passo com driver **A4988**, mantendo a fila de pedidos da v02.

## Contexto — como surgiu o Wokwi

Após a v02, o projecto precisava do **Arduino Mega**, mas o Tinkercad não oferecia condições adequadas para simular essa plataforma.

Durante **novembro**, surgiu uma **fase de indefinição técnica**: ainda não estava definida qual a ferramenta de simulação a adoptar para o Mega.

O orientador **Rui Barros**, por iniciativa própria (sem pedido prévio), fez pesquisa sobre alternativas e enviou uma mensagem no **WhatsApp** a recomendar o **Wokwi**. Essa indicação permitiu retomar o desenvolvimento com uma plataforma adequada.

## Motivo principal da mudança

- **Arduino Mega** — mais GPIO para interiores, porta, reset, mais LEDs e sensores
- Tinkercad limitado ao Uno para esta fase
- Wokwi passou a ser a plataforma de simulação para todo o desenvolvimento seguinte

## O que foi feito

- Transição Tinkercad → Wokwi
- Substituição motor DC por stepper + A4988 (STEP/DIR)
- Controlo não bloqueante do motor
- LCD 20×4 I2C mantido
- Velocidades em passos/segundo
- Mantido array `pedidos[]` da v02

## Decisões técnicas

- Mega como MCU de referência até à maquete real
- A4988 adequado ao ambiente Wokwi
- Wokwi adoptado como simulador principal — **hoje utilizado para tudo**

## Dificuldades

- **Indefinição temporária** sobre a plataforma de simulação do Mega (resolvida com a indicação do orientador)
- Ajuste de velocidades stepper face ao motor DC da fase anterior
- Após adoptar o Wokwi: integração **sem problemas relevantes**

## Resultado / validação

- Simulação estável com movimento por passos
- Fila de pedidos operacional no novo hardware
- Wokwi confirmado como ferramenta central do projecto (v03–v06)

## Evidências

| Ficheiro | Descrição |
|----------|-----------|
| [wokwi/](../../../elevador/simulacao/legacy/wokwi/v03_transicao_mega_a4988/wokwi/) | Export completo |
| [imagem_circuito.png](../../../elevador/simulacao/legacy/wokwi/v03_transicao_mega_a4988/imagem_circuito.png) | Vista do circuito |

## Próximo passo

→ [E04 — 7 segmentos + rearme](E04_wokwi_v04.md)
