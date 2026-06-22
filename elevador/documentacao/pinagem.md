# Pinagem — Arduino Mega (elevador)

> Referência consolidada para simulação v06 e maquete física.  
> Esquemas formais: [ELV-SCH-001](../hardware/esquemas/elevador_v06_forca_comando.svg) · [ELV-SCH-002](../hardware/esquemas/elevador_real_forca_comando.svg)

## Entradas digitais

| Função | Pino | Sim (v06) | Real (maquete) | Lógica activa |
|--------|------|-----------|----------------|---------------|
| Botão exterior P1–P4 | D2–D5 | Botão | Botão | LOW |
| Reset / rearme | D7 | Botão | Botão | LOW |
| Sensor piso 1 | D8 | Interruptor | Hall | LOW |
| Sensor piso 2 | D11 | Interruptor | Hall | LOW |
| Sensor piso 3 | D12 | Interruptor | Hall | LOW |
| Sensor piso 4 | D13 | Interruptor | Hall | LOW |
| Permissiva porta | D22 | Interruptor | Hall | **Sim: HIGH fechada** · **Real: LOW fechada** |
| Botão interior P1–P4 | D23–D26 | Botão | Botão | LOW |

Todas as entradas: `INPUT_PULLUP` (real) ou `INPUT` com pull-up externo/simulado (v06 porta).

## Saídas digitais

| Função | Pino | Sim (v06) | Real (maquete) |
|--------|------|-----------|----------------|
| Buzzer | D6 | Buzzer | Buzzer |
| LED interior P1–P4 | D27–D30 | LED + R | LED + R ~1 kΩ |
| LED exterior P1–P4 | A0–A3 | LED + R | LED + R ~1 kΩ |

## Motor

| Função | Pino | Sim (v06) | Real (maquete) |
|--------|------|-----------|----------------|
| STEP / IN1 | D9 | A4988 STEP | L298N IN1 |
| DIR / IN2 | D10 | A4988 DIR | L298N IN2 → **D33** |
| — | D31 | — | L298N IN3 |
| — | D32 | — | L298N IN4 |

## I2C

| Função | Pino | Sim (v06) | Real (maquete) |
|--------|------|-----------|----------------|
| SDA | D20 | OLED SSD1306 (directo) | TCA9548A → SH1106 |
| SCL | D21 | idem | idem |

- **TCA9548A:** endereço `0x70`; canais 0–3 (um OLED por piso)
- **SH1106:** endereço `0x3C` por canal

## Firmware de referência

| Variante | Ficheiro |
|----------|----------|
| Simulação v06 | `elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/sketch.ino` |
| Maquete real | `elevador/real/l298n_sh1106_hall/src/main.ino` |
