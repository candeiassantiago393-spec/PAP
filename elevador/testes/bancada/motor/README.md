# Teste — Motor L298N + NEMA 17

Validação isolada do motor de passo bipolar via driver L298N.

## Pinagem (Mega → L298N)

| Sinal | Pino |
|-------|------|
| IN1 | D9 |
| IN2 | D33 |
| IN3 | D31 |
| IN4 | D32 |
| GND | GND comum com fonte 12 V |

Alimentação motor: **fonte 12 V** separada (+V → L298N, GND comum). Ver [pinagem](../../../documentacao/pinagem.md).

## Comportamento

- **`s`** — para e **mantém posição** (bobinas activas, torque de retenção)
- **`l`** — liberta bobinas (sem torque)

## Comandos Serial (9600 baud)

| Tecla | Acção |
|-------|--------|
| `u` | Sobe contínuo |
| `d` | Desce contínuo |
| `s` | Para e segura |
| `l` | Liberta bobinas |
| `+` / `-` | Ajusta velocidade (20–400 passos/s) |
| `n200` | 200 passos a subir |
| `p200` | 200 passos a descer |
| `a` | Demo automática |

## Upload

```bash
cd elevador/testes/bancada/motor
pio run -t upload --upload-port COM8
pio device monitor
```

## Etapa

[E07 — Testes de bancada](../../../../docs/ETAPAS/relatorios/E07_testes_bancada.md)
