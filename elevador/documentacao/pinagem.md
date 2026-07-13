# Pinagem — Arduino Mega (elevador)

> Referência consolidada para simulação v06 e maquete física.  
> Esquemas formais: [ELV-SCH-001](../hardware/esquemas/elevador_v06_forca_comando.svg) · [ELV-SCH-002](../hardware/esquemas/elevador_real_forca_comando.svg)

## Entradas digitais

| Função | Sim (v06) | Maquete | Lógica activa |
|--------|-----------|---------|---------------|
| Botão exterior P1–P4 | D2–D5 | D2–D5 | LOW |
| Reset / rearme | D7 | **D10** | LOW |
| Sensor Hall piso 1 | D8 | **D7** | LOW |
| Sensor Hall piso 2 | D11 | **D8** | LOW |
| Sensor Hall piso 3 | D12 | **D11** | LOW |
| Sensor Hall piso 4 | D13 | **D12** | LOW |
| Permissiva porta | D22 | **D22** (chave deslizante) | LOW = fechada |
| Botão interior P1–P4 | D23–D26 | D23–D26 | LOW |

Todas as entradas: `INPUT_PULLUP` (real) ou `INPUT` com pull-up externo/simulado (v06 porta).

### Sensor Hall A3144 (real)

| Campo | Valor |
|-------|-------|
| Tipo | Digital, TO-92 |
| Lógica | **LOW** = íman presente (cabine no piso) |
| Alimentação | 5 V + GND; saída → GPIO com pull-up interno |
| Íman | Na **cabine** — montagem pendente |

### Porta — chave deslizante D22

| Campo | Valor |
|-------|-------|
| Tipo | Chave deslizante (contacto seco) |
| Pino | **D22** |
| Lógica | **LOW** = porta **fechada** (permissiva OK) · **HIGH** = aberta |
| Cablagem | Um terminal → **GND**; outro → **D22** (`INPUT_PULLUP`) — fechada liga D22 a GND |

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

- Driver: **L298N** — motor NEMA 17 bipolar, 4 fios (OUT1/OUT2 + OUT3/OUT4)
- Alimentação motor: **12 V** fonte separada; GND comum Mega + L298N
- Paragem: firmware mantém **bobinas activas** (retenção de posição)
- Teste isolado: [motor](../testes/bancada/motor/)

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
| Maquete (activo) | `elevador/real/codigo_final_3_0_maquete/ext_porta/src/main.ino` |
| Completo (congelado) | `elevador/real/codigo_final_3_0/src/main.ino` |
