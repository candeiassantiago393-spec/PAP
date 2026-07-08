# Testes de Bancada

Validação de hardware real antes da maquete. Ver [E07 — Testes de bancada](../../../docs/ETAPAS/relatorios/E07_testes_bancada.md).

## Metodologia

1. Cada componente testado **isoladamente** (breadboard + Mega)
2. Depois **integração parcial** (`integrado/`) e **firmware completo** (`../../real/l298n_sh1106_hall/`)

## Estrutura (PlatformIO)

Cada pasta tem `src/main.ino` + `platformio.ini` + `README.md`. Ficheiros antigos da Arduino IDE estão em `legacy/`.

| Pasta | Teste | Serial |
|-------|-------|--------|
| [pedidos_exteriores/](pedidos_exteriores/) | Botões D2–D5 + LEDs A0–A3 (pedido latched) | 9600 |
| [oled/](oled/) | Diagnóstico I2C — TCA9548A + 4× SH1106 | 9600 |
| [sensores_hall/](sensores_hall/) | 4× Hall A3144 + gráfico Python | 115200 |
| [motor/](motor/) | NEMA 17 via L298N — comandos por tecla | 9600 |
| [integrado/](integrado/) | Hall + OLED + botões/LEDs ext. (sem motor/porta) | 9600 |

## Upload rápido

```powershell
cd elevador/testes/bancada

# Compilar tudo
.\build_all.ps1

# Enviar para Arduino (detecta COM automaticamente)
.\upload.ps1 motor
.\upload.ps1 integrado
.\upload.ps1 pedidos_exteriores
.\upload.ps1 real
```

Ou manualmente:

```bash
cd elevador/testes/bancada/<pasta>
pio run -t upload --upload-port COM8
pio device monitor
```

## Ordem recomendada na bancada

1. `pedidos_exteriores` — botões e LEDs
2. `oled` — barramento I2C e displays
3. `sensores_hall` — sensores de piso (+ `plot_hall.py`)
4. `motor` — L298N e retenção de posição
5. `integrado` — tudo excepto motor
6. Firmware real — `elevador/real/l298n_sh1106_hall/`

## Imagens

| Ficheiro | Descrição |
|----------|-----------|
| [bancada_integracao_fev_mar_2026.png](imagens/bancada_integracao_fev_mar_2026.png) | Integração completa — fev/mar 2026 |

## Período

**Fevereiro – Julho 2026**

**Nota:** testes com carga real atrasados pela entrega da fonte de alimentação — ver E07.
