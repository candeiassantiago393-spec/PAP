# Esquemas elétricos — Elevador 4 pisos

Documentação formal dos circuitos de **força** (potência) e **comando** (lógica), no formato utilizado em automatismos industriais.

| Desenho | Ficheiro | Variante | Rev. |
|---------|----------|----------|------|
| **ELV-SCH-001** | [elevador_v06_forca_comando.svg](elevador_v06_forca_comando.svg) | Simulação Wokwi v06 | A |
| **ELV-SCH-002** | [elevador_real_forca_comando.svg](elevador_real_forca_comando.svg) | Maquete física E08/E09 | A |

### Referências complementares

| Ficheiro | Descrição |
|----------|-----------|
| [esquematico_v06_wokwi.svg](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/esquematico_v06_wokwi.svg) | Export Wokwi (ligação fio-a-fio) |
| [imagem_circuito.png](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/imagem_circuito.png) | Captura do circuito simulado |
| [diagram.json](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/diagram.json) | Ligações exactas Wokwi |
| [pinagem.md](../../documentacao/pinagem.md) | Mapa de pinos consolidado |

---

## Estrutura geral

```text
┌──────────────────────────────┬──────────────────────────────┐
│   CIRCUITO DE FORÇA          │   CIRCUITO DE COMANDO          │
│   Alimentação · Motor · I2C  │   Entradas · Saídas · MCU      │
├──────────────────────────────┼──────────────────────────────┤
│  +12 V → F1 → Driver → M1    │  Pedidos ext./int.             │
│  +5 V  → Mega · periféricos  │  Sensores piso / porta         │
│  GND comum                   │  Arduino Mega → driver motor   │
│  I2C → displays              │  LEDs · buzzer · reset         │
└──────────────────────────────┴──────────────────────────────┘
```

**Nota de desenho:** os símbolos SP (porta) e S0 (reset) no circuito de comando representam **interlocks funcionais** implementados no firmware — não constituem uma cadeia eléctrica em série.

---

## ELV-SCH-001 — Simulação Wokwi v06

### Circuito de força

| Ref. | Componente | Função |
|------|------------|--------|
| PSU | Fonte 12 V / 5 V (USB na sim) | Alimentação |
| F1 | Fusível 2 A gG | Protecção ramo motor |
| A4988 | Driver stepper | STEP/DIR → corrente nas bobinas |
| M1 | NEMA 17 bipolar | Movimento cabine (4 fios) |
| U1 | Arduino Mega 2560 | Controlador |
| — | 4× OLED SSD1306 | Display por piso (I2C directo) |

#### Ligações bobinas (diagram.json v06)

| Motor | A4988 |
|-------|-------|
| A+ | 2A |
| A− | 2B |
| B+ | 1A |
| B− | 1B |

### Circuito de comando

| Função | Pino | Tipo | Notas |
|--------|------|------|-------|
| Pedidos exterior P1–P4 | D2–D5 | Entrada | `INPUT_PULLUP`, LOW = activo |
| Pedidos interior P1–P4 | D23–D26 | Entrada | idem |
| Sensores piso | D8, D11–D13 | Entrada | Interruptor, LOW = activo |
| Permissiva porta | D22 | Entrada | **HIGH = fechada** (sim) |
| Reset / rearme | D7 | Entrada | LOW = premido |
| STEP | D9 | Saída | → A4988 |
| DIR | D10 | Saída | → A4988 |
| Buzzer | D6 | Saída | — |
| LEDs exteriores | A0–A3 | Saída | + R ~1 kΩ |
| LEDs interiores | D27–D30 | Saída | + R ~1 kΩ |
| I2C SDA / SCL | D20 / D21 | I2C | → OLEDs |

**Firmware:** [sketch.ino](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/sketch.ino)

---

## ELV-SCH-002 — Maquete física

### Circuito de força

| Ref. | Componente | Função |
|------|------------|--------|
| PSU | Orno 12 VDC 5 A 60 W | Alimentação industrial |
| F1 | Fusível 2 A gG | Protecção ramo motor |
| L298N | Ponte H dupla | IN1–IN4 → OUT1–OUT4 → bobinas |
| M1 | NEMA 17 bipolar | Acoplado cabo/tambor |
| U1 | Arduino Mega 2560 | Controlador |
| TCA9548A | Multiplexador I2C 0x70 | 4 canais para 4 displays |
| — | 4× OLED SH1106 1,3" | Display por piso (canal 0–3) |
| — | 5× Sensor Hall | 4 pisos + porta |

#### Ligações L298N

| L298N | Arduino Mega | Motor |
|-------|--------------|-------|
| IN1 | D9 | — |
| IN2 | D33 | — |
| IN3 | D31 | — |
| IN4 | D32 | — |
| OUT1 | — | Bobina A+ |
| OUT2 | — | Bobina A− |
| OUT3 | — | Bobina B+ |
| OUT4 | — | Bobina B− |
| ENA | +5 V | Sempre activo |
| VS | +12 V | Via F1 |
| GND | GND comum | Mega + fonte |

**Requisitos de montagem:**
- GND comum entre fonte 12 V, L298N e Mega (obrigatório)
- Não alimentar o motor apenas pelo USB do Mega
- Ventoinha 40 mm no L298N recomendada
- Jumpers de lógica L298N em 5 V

#### Sensores Hall

| Função | Pino | Lógica |
|--------|------|--------|
| Piso 1 | D8 | LOW = íman detetado |
| Piso 2 | D11 | idem |
| Piso 3 | D12 | idem |
| Piso 4 | D13 | idem |
| Permissiva porta | D22 | **LOW = porta fechada** (real) |

Alimentação Hall: VCC → +5 V, GND → barramento, OUT → GPIO com `INPUT_PULLUP`.

**Firmware:** [src/main.ino](../../real/l298n_sh1106_hall/src/main.ino) · `pio run`

---

## Comparação simulação ↔ maquete

| Aspeto | ELV-SCH-001 (v06) | ELV-SCH-002 (real) |
|--------|-------------------|---------------------|
| Driver motor | A4988 (STEP/DIR) | L298N (4× IN) |
| Pinos motor | D9, D10 | D9, D31, D32, D33 |
| Display | SSD1306, I2C directo | SH1106 via TCA9548A |
| Sensor piso | Interruptor | Hall |
| Porta fechada | D22 = HIGH | D22 = LOW |
| Alimentação | USB sim | Fonte 12 V Orno 5 A |
| Estados firmware | v06 Wokwi | v06 adaptado (E09) |

---

## Legenda de condutores (ambos os desenhos)

| Cor / estilo | Significado |
|--------------|-------------|
| Vermelho sólido | +12 V (potência motor) |
| Azul sólido | +5 V (lógica) |
| Preto sólido | GND comum |
| Cinza tracejado | Sinal digital / I2C |

---

## Estados do sistema

Ver [estados_sistema.md](../../documentacao/estados_sistema.md): NORMAL · PORTA · PARAGEM · ESTAB · ERRO_MOV · ERRO_PAR · REARME

---

## Referências e etapas

| Etapa | Ligação |
|-------|---------|
| E06 — Simulação v06 | [relatório](../../../docs/ETAPAS/relatorios/E06_wokwi_v06.md) |
| E08 — Montagem maquete | [relatório](../../../docs/ETAPAS/relatorios/E08_montagem_maquete.md) |
| E09 — Firmware real | [relatório](../../../docs/ETAPAS/relatorios/E09_firmware_real.md) |

---

## Pendências

- [ ] Export PDF (ELV-SCH-001 e ELV-SCH-002) para relatório PAP
- [ ] Esquema de ligação detalhado (Fritzing/KiCad) com resistências e bornes
- [ ] Folha 2: esquema mecânico cabine + tambor (fase posterior)
