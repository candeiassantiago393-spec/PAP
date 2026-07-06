# Pinagem — Estendal Inteligente (Arduino Uno)

| Função | Pino | Notas |
|--------|------|-------|
| Sensor chuva (sim: slide switch) | D2 | INPUT — HIGH = chuva |
| Servo sinal (PWM) | D9 | `Servo.write()` |
| Servo V+ | 5V | Alimentação |
| Servo GND | GND | Massa |

## Sensor chuva real (Temu)

Confirmar na montagem:

- Saída digital (DO) ou analógica (AO)?
- Tensão de alimentação (3.3 V vs 5 V)
- Lógica: HIGH ou LOW quando chove?

Gravar pinagem definitiva neste ficheiro após testes na bancada.

## Alimentação servo

Servos consomem pico de corrente — considerar fonte externa 5 V se o Uno resetar durante movimento.
