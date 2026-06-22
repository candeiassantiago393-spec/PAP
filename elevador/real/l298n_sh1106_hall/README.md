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
| Porta | Interruptor D22 | Hall D22 |

## Otimização I2C / motor

O código actualiza **um OLED de cada vez** e só quando há mudança — reduz interferência no barramento I2C que causava gaguejar do motor (ver E07).

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
| `src/main.ino` | Firmware PlatformIO (fonte principal) |
| `elevador_real_....ino` | Cópia com nome legado |
| `platformio.ini` | Configuração Mega + bibliotecas |

## Simulação de referência

[Wokwi v06](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/)
