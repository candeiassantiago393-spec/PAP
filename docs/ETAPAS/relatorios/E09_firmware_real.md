# E09 — Firmware Real (L298N + Hall + SH1106)

| Campo | Valor |
|-------|-------|
| **Estado** | Em desenvolvimento / testes na maquete |
| **Código** | [elevador/real/l298n_sh1106_hall](../../../elevador/real/l298n_sh1106_hall/) |
| **Compilar** | `pio run` nesta pasta (PlatformIO) |
| **Referência sim** | [Wokwi v06](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/) |

## Objetivo

Firmware da **maquete física** — mesma lógica da v06, hardware real.

## Diferenças simulação → real

| Componente | v06 Wokwi | Maquete real |
|------------|-----------|--------------|
| Driver motor | A4988 | L298N (IN1–IN4) |
| Display | SSD1306 | SH1106G |
| Sensor piso | Interruptor | Hall (LOW) |
| Porta | Interruptor | Hall D22 |
| Alimentação | USB sim | Fonte 12 V 5 A |

## O que foi feito

- Adaptação completa do código v06 para L298N + SH1106
- Otimização refresh OLED (1 display/vez, só em mudança) — resolve gaguejar motor
- Pinagem consolidada no `.ino`
- Projeto **PlatformIO** para compilar no Cursor

## Decisões técnicas

- `src/main.ino` como fonte principal PlatformIO
- Manter cópia com nome legado para referência
- Bibliotecas: Adafruit GFX + Adafruit SH110X

## Dificuldades

- Sincronizar duas bases de código (sim vs real) — pastas separadas
- I2C vs motor — resolvido no refactor OLED (E07)

## Resultado / validação

- Bancada: aprovado
- Maquete: integração em curso (E08)
- **Compilação PlatformIO** (22/06/2026): `pio run` — **SUCCESS**
  - RAM: 13,5% (1108 / 8192 bytes)
  - Flash: 8,4% (21316 / 253952 bytes)
  - Board: Arduino Mega 2560

## Evidências

- [Firmware](../../../elevador/real/l298n_sh1106_hall/src/main.ino)
- [platformio.ini](../../../elevador/real/l298n_sh1106_hall/platformio.ini)

## Compras relacionadas

- Fonte, OLEDs: [2026EC1454381](../../compras/faturas_mauser/2026EC1454381.md)
- Motor, L298N, Hall: _faturas pendentes_

## Próximo passo

→ Completar montagem E08 (Hall, motor) e comissionamento E10
