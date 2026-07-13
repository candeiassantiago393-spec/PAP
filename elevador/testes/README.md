# Testes — Elevador

Validação isolada de hardware antes da integração na maquete.

## Bancada ([bancada/](bancada/))

| Pasta | Componente |
|-------|------------|
| pedidos_exteriores | Botões + LEDs exteriores |
| sensores_hall | Hall A3144 + gráfico Python |
| hall_diag / hall_sequencia | Diagnóstico e sequência motor+Hall |
| hall_porta / reset_d10 | Porta D22 e reset D10 |
| motor_hall_pedidos | Integração pedidos + motor |
| oled | Displays (versão completa) |

Upload: `cd elevador/testes/bancada && .\upload.ps1 <nome>`

## Virtual ([virtual/](virtual/))

Reservado para testes sem hardware.

## Navegação

| | |
|--|--|
| Elevador | [../README.md](../README.md) |
| Firmware maquete | [../real/codigo_final_3_0_maquete/ext_porta/](../real/codigo_final_3_0_maquete/ext_porta/) |
| E07 | [../../docs/ETAPAS/relatorios/E07_testes_bancada.md](../../docs/ETAPAS/relatorios/E07_testes_bancada.md) |
