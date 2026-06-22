# E06 — Wokwi v06 — Sistema Completo Simulado

| Campo | Valor |
|-------|-------|
| **Estado** | Concluída |
| **Período** | Janeiro 2026 |
| **Código** | [elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta](../../../elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/) |
| **Wokwi** | https://wokwi.com/projects/464344193596402689 |
| **Referência** | **Versão oficial de simulação** (apresentação e baseline lógico) |

## Objetivo

Completar a lógica do elevador real na simulação: pedidos interiores, permissiva de porta, paragem temporizada e fila unificada interior/exterior.

## O que foi feito

- Botões interiores da cabine (D23–D26) + LEDs interiores (D27–D30)
- Permissiva de porta (D22) com debounce
- Estado `PORTA` nos OLEDs — motor bloqueado com porta aberta
- Paragem temporizada de **10 s** em piso pedido
- Fila comum para pedidos exteriores e interiores
- Atendimento de pedido no piso actual
- Mantidos: OLEDs, TCA9548A, segurança v04/v05, A4988

## Decisões técnicas

- **v06** definida como referência para firmware real e comissionamento da maquete
- Pinagem consolidada — ver [CONTEXTO_PROJETO.md](../../CONTEXTO_PROJETO.md)

## Dificuldades

- Extensão natural da v05 — integração de I/O adicional sem alteração de plataforma
- Ajustes de debounce da porta e sincronização com estados nos OLEDs

## Resultado / validação

- Simulação representa o comportamento esperado do elevador real
- Pronta para demonstração no Cursor (extensão Wokwi)
- Base para testes de bancada (E07) e montagem (E08)

## Evidências

| Ficheiro | Descrição |
|----------|-----------|
| [wokwi/](../../../elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/) | Export completo |
| [imagem_circuito.png](../../../elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/imagem_circuito.png) | Vista do circuito |

## Próximo passo

→ [E07 — Testes de bancada](E07_testes_bancada.md)
