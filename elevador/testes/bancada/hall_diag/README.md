# Diagnóstico Hall — pisos 1–4

Confirma se **D7, D8, D11, D12** mudam quando magnetizas cada sensor.

| Piso | Pino |
|------|------|
| 1 | D7 |
| 2 | D8 |
| 3 | D11 |
| 4 | D12 |

## Upload

```powershell
cd elevador/testes/bancada
.\upload.ps1 hall_diag
pio device monitor
```

## Interpretação

| Resultado | Causa provável |
|-----------|----------------|
| Piso 3/4 OK, 1/2 LED acende mas **D7/D8=HIGH** | Fio **OUT** dos módulos 1/2 não ligado a D7/D8 |
| **D7=LOW** sempre (sem íman) | Curto a GND, reset ainda em D7, ou fio OUT em curto |
| Sensor 1 funciona em **D11** (teste troca cabos) | Cabo OUT do piso 1 ligado ao pin errado |
| OUT→GND manual e D7 muda | Mega OK — repara cabo/modulo Hall |

## Teste de troca (5 min)

1. **Desliga** o fio OUT do sensor **1** do D7
2. Liga temporariamente esse OUT ao **D11** (onde o piso 3 funciona)
3. Magnetiza sensor 1 → se **D11** muda, o **módulo 1 está OK** e o problema é o caminho até **D7**

## Cablagem correcta módulo Hall

```
VCC -> 5V     GND -> GND     OUT -> D7 ou D8 (etc.)
```

O **LED do módulo** acende só com VCC+GND+ímã — **não prova** que OUT está ligado.
