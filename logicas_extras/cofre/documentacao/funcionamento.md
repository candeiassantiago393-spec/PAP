# Funcionamento — Cofre código secreto

## Variáveis principais

| Variável | Valor / tipo | Descrição |
|----------|--------------|-----------|
| `codigoCorreto` | `"1904"` | PIN válido (hardcoded na v1) |
| `input` | `String` | Dígitos introduzidos desde o último reset |

## Fluxo

```text
Tecla numérica ──► concatena em input
Tecla * ──► input = "" (reset)
Tecla # ──► compara input com codigoCorreto
              ├─ igual  → LED verde, servo 90°, bip 1 kHz
              └─ diferente → LED vermelho, bip 200 Hz
            input = ""
```

## Estados do servo

| Posição | Significado |
|---------|-------------|
| `0°` | Cofre fechado (início) |
| `90°` | Cofre aberto (após código correcto) |

## Serial Monitor (9600 baud)

- Cada tecla pressionada
- `Codigo atual: ...` durante introdução
- `ACESSO OKAY` ou `ACESSO NEGADO`
- `RESET` ao carregar `*`

## Melhorias futuras

- Timeout se não confirmar em X segundos
- Limite de tentativas erradas
- Fechar servo automaticamente após abertura
- Código configurável (EEPROM ou menu)
