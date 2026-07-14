# Pinagem — Código Prédio (Arduino Uno)

Teclado Mauser **096-4642** (3×4) — fatura [2026EC1403323](../../../docs/compras/faturas_mauser/2026EC1403323.md).

| Função | Pino |
|--------|------|
| Teclado — fila 1 (R1) | D5 |
| Teclado — fila 2 (R2) | D4 |
| Teclado — fila 3 (R3) | D3 |
| Teclado — fila 4 (R4) | D2 |
| Teclado — coluna 1 (C1) | D8 |
| Teclado — coluna 2 (C2) | D7 |
| Teclado — coluna 3 (C3) | D6 |
| Servo (PWM) | D9 |

## Layout do teclado

```text
┌───┬───┬───┐
│ 1 │ 2 │ 3 │
├───┼───┼───┤
│ 4 │ 5 │ 6 │
├───┼───┼───┤
│ 7 │ 8 │ 9 │
├───┼───┼───┤
│ * │ 0 │ # │
└───┴───┴───┘
```

- `*` — apagar código introduzido
- `#` — confirmar e validar
