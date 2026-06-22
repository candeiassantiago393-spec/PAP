# Estados do sistema — Elevador

Estados implementados no firmware v06 (simulação) e firmware real.

| Estado | Descrição |
|--------|-----------|
| `NORMAL` | Operação — atende fila de pedidos, move entre pisos |
| `PORTA` | Paragem em piso pedido; porta aberta; motor bloqueado |
| `PARAGEM` | Transição de paragem / estabilização |
| `ESTAB` | Estabilização após chegada ao piso |
| `ERRO_MOV` | Erro de movimento (timeout, falha de deslocamento) |
| `ERRO_PAR` | Erro de paragem (não detetou piso esperado) |
| `REARME` | Aguarda acção do operador após erro (botão reset) |

## Fluxo simplificado

```text
NORMAL ──pedido/piso──► movimento ──► ESTAB ──► PORTA (10 s) ──► NORMAL
   │                                      │
   └── erro ──► ERRO_* ──► REARME ──reset──┘
```

## Referência de código

- Simulação: `simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/sketch.ino`
- Real: `real/l298n_sh1106_hall/src/main.ino`
