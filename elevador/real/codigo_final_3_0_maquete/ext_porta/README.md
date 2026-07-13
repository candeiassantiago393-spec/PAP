# ext_porta — pedidos exteriores + porta D22

Firmware **activo e testado** na maquete (bancada / Agatha).

Sem OLED. Sem botões interiores. Motor L298N + Hall + permissiva de porta.

## Estado (testes)

| Funcionalidade | Estado |
|----------------|--------|
| Vários pedidos (fila SCAN) | Testado |
| Bobinas desligadas no piso 1 (tábua de apoio) | Testado |
| Lógica de porta (D22 — só move fechada) | Testado |
| Sensores Hall calibrados (debounce 120 ms) | Testado |
| Buzzer (pedido + alarme) | Testado |
| Modo emergência / rearme (Reset D10) | **Por testar** |

## Por implementar (outras variantes futuras)

- Displays OLED (4× SH1106)
- Pedidos interiores (D23–D26)

## Pinagem

```
Botões ext:  D2–D5    LEDs ext: A0–A3
Hall pisos:  D7, D8, D11, D12  (LOW = íman)
Porta:       D22  (LOW = fechada, HIGH = aberta)
Reset:       D10
Buzzer:      D6
L298N:       D9, D33, D31, D32
```

## Tempos

| Parâmetro | Valor |
|-----------|-------|
| Paragem com pedido | 10 s (+ porta fechada para continuar) |
| Estabilização destino | 300 ms |
| Rearme espera piso 1 | 3 s (teste; produção: 10 s) |
| Timeout movimento | 45 s |

## Upload

```powershell
cd elevador/real/codigo_final_3_0_maquete/ext_porta
pio run -t upload
pio device monitor -b 9600
```

Ou a partir da bancada:

```powershell
cd elevador/testes/bancada
.\upload.ps1 maquete
```

## Navegação

| | |
|--|--|
| Índice maquete | [../README.md](../README.md) |
| Versão completa (OLED + int.) | [../../codigo_final_3_0/](../../codigo_final_3_0/) |
| Pinagem | [../../../documentacao/pinagem.md](../../../documentacao/pinagem.md) |
