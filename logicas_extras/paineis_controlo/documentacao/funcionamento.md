# Funcionamento — Caixa ESP32 OLED

Painel multi-ecrã que agrega várias **lógicas extras** do prédio numa única interface (demo apresentação PAP).

## Ecrãs

| # | Nome | Subsistema PAP | Descrição |
|---|------|----------------|-----------|
| 1 | HOME | Painel | Marca candeias.dev / studio |
| 2 | AMBIENTE | [Meteorologia](../../meteorologia/) | DHT22 — temperatura e humidade |
| 3 | MOVIMENTO | [Segurança](../../seguranca/) | PIR — alerta intruso, buzzer |
| 4 | PLANTAS | Estufa / rega | Humidade solo, LEDs, bomba temporizada |
| 5 | GRÁFICO | Meteorologia | Sparkline temperatura |
| 6 | Instagram | Portfolio | QR code @candeias.dev |

## Navegação

- **NEXT (GPIO 32)** — ecrã seguinte (ciclo 1→6)
- **OK (GPIO 33)** — silencia alarme durante alerta PIR

## Modo feira

- Rotação automática dos ecrãs a cada **5 s**
- Pausa ao carregar botões
- Retoma após **2 min** sem interação

## Alertas Telegram (opcional)

PIR (intruso) e solo SECO (&lt; 35 %). Configuração: [real/caixa_ecra/TELEGRAM.md](../real/caixa_ecra/TELEGRAM.md).

Lógica estufa baseada em [Projeto-Estufa v1.3](https://github.com/candeiassantiago393-spec/Projeto-Estufa).

## Firmware

- Simulação: [simulacao/wokwi/sketch.ino](../simulacao/wokwi/sketch.ino)
- Real / PlatformIO: [real/caixa_ecra/src/main.cpp](../real/caixa_ecra/src/main.cpp)
