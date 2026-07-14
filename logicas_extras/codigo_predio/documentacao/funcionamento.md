# Funcionamento — Código Porta Prédio

## Variáveis principais

| Variável | Valor | Descrição |
|----------|-------|-----------|
| `codigoCorreto` | `"1904"` | PIN válido |
| `input` | `String` | Dígitos desde o último reset |

## Fluxo

```text
Tecla numérica ──► concatena em input
Tecla * ──► input = "" (reset)
Tecla # ──► compara input com codigoCorreto
              ├─ igual  → servo 0→90° (suave, delay 15 ms/passo)
              │           espera 8 s
              │           servo 90→0° (suave)
              └─ diferente → servo não move
            input = ""
```

## Estados do servo

| Posição | Significado |
|---------|-------------|
| `0°` | Porta/fechadura fechada |
| `90°` | Aberta (durante os 8 s após código correcto) |

## Serial Monitor (9600 baud)

- Cada tecla pressionada
- `Codigo atual: ...` durante introdução
- `ACESSO OK` ou `ACESSO NEGADO`
- `RESET` ao carregar `*`

## Melhorias futuras

- Timeout se não confirmar em X segundos
- Limite de tentativas erradas
- Código configurável (EEPROM)
- Migrar para ESP32 + teclado físico Mauser
