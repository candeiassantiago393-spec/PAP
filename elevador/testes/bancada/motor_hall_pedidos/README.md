# Teste bancada — Motor + Hall + pedidos exteriores

Elevador simplificado: **só** botões/LEDs exteriores, sensores Hall e motor L298N.  
Lógica de fila e paragem nos pisos extraída do [firmware maquete](../../../real/codigo_final_3_0_maquete/) — sem OLED, botões interiores, buzzer, reset ou rearme.

## Pinagem Mega

| Função | Pin |
|--------|-----|
| Botões ext. | D2, D3, D4, D5 |
| LEDs ext. | A0, A1, A2, A3 |
| Hall pisos | D7, D8, D11, D12 (LOW = activo) |
| L298N IN1–IN4 | D9, D33, D31, D32 |

## Comportamento

1. **Arranque:** assume **piso 1** (cabine já lá) — **motor parado**; se Hall activo, confirma o piso
2. **Pedido:** premir botão exterior → `pedidos[i]=true` → **LED acende** (A0–A3)
3. Motor segue **SCAN** até ao piso pedido
4. **Atendido:** ao chegar → `pedidos[i]=false` → **LED apaga** → para **5 s**
5. Fila vazia → regressa ao **piso 1** e espera

## Upload

```powershell
cd elevador/testes/bancada/motor_hall_pedidos
pio run -t upload --upload-port COMx
pio device monitor
```

Ou: `..\upload.ps1 motor_hall_pedidos`

## Critérios de sucesso

- [ ] Arranque parado no piso 1 (sem movimento)
- [ ] Pedido no piso 3 → motor move e para no 3
- [ ] Vários pedidos → atende por ordem de sentido
- [ ] Motor mantém posição parado (bobinas activas)
