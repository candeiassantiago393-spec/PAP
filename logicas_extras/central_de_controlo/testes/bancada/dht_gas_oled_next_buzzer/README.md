# Teste — KY-015 + MQ gás + OLED + NEXT + buzzer (Mega)

Junção combinada: **temperatura/humidade**, **gás**, **display**, **botão NEXT**, **buzzer**.

## Ecrãs (NEXT)

| Ecrã | Conteúdo |
|------|----------|
| HOME | `candeias.dev studio` |
| GRAFICOS | Gráfico temp (cima) + gráfico hum (baixo) |
| BARRAS | Barra temp (cima) + barra hum (baixo) |
| GAS | Barra gás + alarme buzzer |

**NEXT** (pin 3) avança HOME → AMBIENTE → GAS → GRAF → HOME.

Alarme gás **≥ 50 %** → ecrã **FUGA DE GAS!!** + **buzzer** (pin 6) ligado.

## Hardware

| Componente | Ligação |
|------------|---------|
| KY-015 | (-) GND, (S) **pin 2**, (+) 5V |
| MQ-2 / MQ-135 | VCC 5V, GND, **AOUT → A1** (só analógico) |
| OLED SSD1306 | SDA **20**, SCL **21**, 0x3C |
| Botão NEXT | **pin 3** ↔ GND |
| Buzzer | **pin 6** + GND |

## Upload

```powershell
cd logicas_extras/central_de_controlo/testes/bancada/dht_gas_oled_next_buzzer
pio run -t upload --upload-port COMx
pio device monitor -b 9600
```

## Versão real

Quando aprovado: copiado para [../../real/versoes/dht_gas_oled_next_buzzer/v1/](../../real/versoes/dht_gas_oled_next_buzzer/v1/) e [../../real/atual/](../../real/atual/).
