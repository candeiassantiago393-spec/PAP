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

## Compilar

```bash
cd elevador/real/l298n_sh1106_hall
pio run
```

## Documentação relacionada

| Documento | Conteúdo |
|-----------|----------|
| [pinagem](../documentacao/pinagem.md) | GPIO Mega — L298N, Hall, OLED |
| [esquema real](../hardware/esquemas/elevador_real_forca_comando.svg) | ELV-SCH-002 |

## Navegação

| | |
|--|--|
| Elevador | [../README.md](../README.md) |
| Simulação v06 | [../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/](../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/) |
| Etapa E09 | [../../docs/ETAPAS/relatorios/E09_firmware_real.md](../../docs/ETAPAS/relatorios/E09_firmware_real.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
