# Implementação Real — Elevador

Firmware e documentação da maquete física.

## Diferenças face à simulação v06

| Componente | Simulação | Real |
|------------|-----------|------|
| Driver motor | A4988 | L298N |
| Display | SSD1306 | SH1106 |
| Sensor piso | Interruptor | Hall |
| Alimentação | USB | 12 V / 5 A |

## Versões

| Pasta | Descrição |
|-------|-----------|
| [l298n_sh1106_hall/](l298n_sh1106_hall/) | Versão atual — motor bipolar, OLEDs SH1106 |

## Migração

Conteúdo integrado no repo `PAP` — ver `elevador/real/l298n_sh1106_hall/`.
