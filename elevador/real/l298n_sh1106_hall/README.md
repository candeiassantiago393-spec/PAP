# Firmware real — L298N + SH1106 + Hall

Versão de **implementação física** da maquete. Base lógica alinhada com Wokwi v06, adaptada ao hardware real.

## Etapa

[E09 — Firmware real](../../../docs/ETAPAS/relatorios/E09_firmware_real.md)

## Diferenças face à simulação v06

| Aspeto | Wokwi v06 | Esta versão |
|--------|-----------|-------------|
| Driver motor | A4988 (STEP/DIR) | L298N (4 fios bobinas) |
| Display | SSD1306 | SH1106G |
| Sensor piso | Interruptor | Hall (LOW = activo) |
| Porta | Interruptor D22 | **Não usada** neste firmware — lógica em [v06](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/) |

## Otimização I2C / motor

O código actualiza **um OLED de cada vez** e só quando há mudança — reduz interferência no barramento I2C que causava gaguejar do motor (ver E07).

Ao parar, o motor **mantém a fase activa** (retenção de posição) — não desliga as bobinas. Teste isolado: [motor](../../testes/bancada/motor/).

## Compilar no Cursor (PlatformIO)

1. Extensão **PlatformIO IDE** instalada
2. Abrir esta pasta: `elevador/real/l298n_sh1106_hall/`
3. Terminal:

```bash
pio run
```

4. Erros aparecem no painel **Problems**
5. Upload (Mega ligado por USB):

```bash
pio run -t upload
pio device monitor
```

## Ficheiros

| Ficheiro | Uso |
|----------|-----|
| `src/main.ino` | **Única fonte** do firmware real |
| `platformio.ini` | Configuração Mega + bibliotecas Adafruit |

> Versão **com porta** (permissiva D22): usar simulação [Wokwi v06](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/) — não duplicar aqui.

## Simulação de referência

[Wokwi v06](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/)
