# Pinagem — Meteorologia (DHT22 no painel ecra)

> Mesma pinagem do [painel ecra](../../paineis_controlo/documentacao/pinagem.md). Meteorologia usa apenas **DHT22** + **OLED I2C**.

## ESP32 — meteorologia

| Função | GPIO | Ligação |
|--------|------|---------|
| DHT22 dados | **26** | SDA do sensor (biblioteca DHTesp) |
| DHT22 VCC | 3V3 | — |
| DHT22 GND | GND | — |
| OLED SDA | **21** | I2C |
| OLED SCL | **22** | I2C |
| Botão NEXT | 32 | Navegar até ecrã AMBIENTE / GRÁFICO |

Endereço OLED: **0x3C** · resolução **128×64** SSD1306.

## Diagrama Wokwi

Componentes no circuito: [paineis_controlo/simulacao/wokwi/diagram.json](../../paineis_controlo/simulacao/wokwi/diagram.json)

- `dht1` — DHT22 ligado a GPIO 26, 3V3, GND

## Compras

| Componente | Código Mauser | Fatura |
|------------|---------------|--------|
| NodeMCU ESP32 USB-C | 095-4900 | [2026EC1403323](../../../docs/compras/faturas_mauser/2026EC1403323.md) |
| OLED 0,96" protótipo | 096-7806 | [2026EC1403323](../../../docs/compras/faturas_mauser/2026EC1403323.md) |

DHT22: incluído na simulação Wokwi; sensor físico a confirmar na BOM da maquete.

## Por andar (planeado)

Cada réplica por piso repete:

```text
ESP32 ── I2C ── OLED 128×64
  │
  └── GPIO 26 ── DHT22
```

Alimentação comum 5 V / 3V3 conforme esquema final da maquete.

## Navegação

| | |
|--|--|
| Funcionamento | [funcionamento.md](funcionamento.md) |
| Módulo | [../README.md](../README.md) |
| Pinagem completa painel | [../../paineis_controlo/documentacao/pinagem.md](../../paineis_controlo/documentacao/pinagem.md) |
