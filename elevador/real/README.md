# Implementação Real — Elevador

Firmware da maquete física (Arduino Mega + L298N + Hall).

## Firmware

| Pasta | Uso | Alterar? |
|-------|-----|----------|
| [codigo_final_3_0/](codigo_final_3_0/) | Versão **completa** — OLED, pedidos int/ext, buzzer, rearme | **Não** (arquivo congelado) |
| [codigo_final_3_0_maquete/](codigo_final_3_0_maquete/) | Versão **maquete** — só pedidos ext., porta D22, pinagem actual | Sim (evolução activa) |

## Diferenças face à simulação v06

| Componente | Simulação | Real |
|------------|-----------|------|
| Driver motor | A4988 | L298N |
| Display | SSD1306 | SH1106 (só versão completa) |
| Sensor piso | Interruptor | Hall A3144 |
| Porta | Interruptor D22 | Chave deslizante D22 (variant maquete) |

## Upload rápido

```powershell
# Maquete (recomendado)
cd elevador/real/codigo_final_3_0_maquete
pio run -t upload
pio device monitor -b 9600

# Versão completa (OLED + interiores)
cd elevador/real/codigo_final_3_0
pio run -t upload
pio device monitor -b 115200
```

## Documentação

| Documento | Conteúdo |
|-----------|----------|
| [pinagem](../documentacao/pinagem.md) | GPIO Mega |
| [estados_sistema](../documentacao/estados_sistema.md) | Máquina de estados |
| [esquema real](../hardware/esquemas/elevador_real_forca_comando.svg) | ELV-SCH-002 |

## Legacy

Versões antigas: [legacy/](legacy/)

## Navegação

| | |
|--|--|
| Elevador | [../README.md](../README.md) |
| Testes bancada | [../testes/bancada/](../testes/bancada/) |
| Simulação v06 | [../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/](../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/) |
