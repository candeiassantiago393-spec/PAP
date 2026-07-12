# Código Final 3.0 — variant maquete

Firmware **activo** para a maquete física: derivação do Código Final 3.0 adaptada ao hardware montado.

## Diferenças face à versão completa

| Aspeto | [codigo_final_3_0](../codigo_final_3_0/) | Esta variant |
|--------|------------------------------------------|--------------|
| OLED SH1106 / TCA9548A | Sim | **Removido** |
| Botões/LEDs interiores | Sim | **Removido** |
| Pedidos exteriores | Sim | Sim |
| Permissiva porta D22 | Não | **Sim** (chave deslizante) |
| Pinagem Hall maquete | D7, D8, D11, D12 | D7, D8, D11, D12 |
| Reset | D10 | D10 |

## Pinagem

```
Botões ext:  D2–D5    LEDs ext: A0–A3
Hall pisos:  D7, D8, D11, D12  (LOW = íman)
Porta:       D22  (LOW = fechada)
Reset:       D10
Buzzer:      D6
L298N:       D9, D33, D31, D32
```

## Comportamento

- Fila SCAN, paragem 10 s no piso pedido
- Motor só move com porta **fechada** (D22 LOW)
- Reset → sequência rearme (alarme → piso 1)
- Timeout movimento 15 s → erro

## Compilar / upload

```powershell
cd elevador/real/codigo_final_3_0_maquete
pio run -t upload
pio device monitor -b 9600
```

Ou a partir da bancada:

```powershell
cd elevador/testes/bancada
.\upload.ps1 maquete
```

## Origem

Copiado de `Codigo_Final_3_0/codigo_final_3.0_semdisplays_sempedidosijnternos.ino` (Desktop).

## Navegação

| | |
|--|--|
| Versão completa (congelada) | [../codigo_final_3_0/](../codigo_final_3_0/) |
| Pinagem | [../../documentacao/pinagem.md](../../documentacao/pinagem.md) |
| Testes bancada | [../../testes/bancada/](../../testes/bancada/) |
