# Testes de Bancada

Validação de hardware real antes/durante montagem na maquete. Ver [E07 — Testes de bancada](../../../docs/ETAPAS/relatorios/E07_testes_bancada.md).

## Testes activos

| Pasta | O que testa | Serial |
|-------|-------------|--------|
| [pedidos_exteriores/](pedidos_exteriores/) | Botões D2–D5 + LEDs A0–A3 | 9600 |
| [sensores_hall/](sensores_hall/) | 4× Hall + gráfico Python | **115200** |
| [hall_diag/](hall_diag/) | Leitura bruta D7, D8, D11, D12 | 9600 |
| [hall_sequencia/](hall_sequencia/) | Motor sobe/desce + detecção pisos | 9600 |
| [hall_porta/](hall_porta/) | Chave porta D22 | 9600 |
| [reset_d10/](reset_d10/) | Botão reset D10 | 9600 |
| [motor/](motor/) | L298N isolado | 9600 |
| [motor_hall_pedidos/](motor_hall_pedidos/) | Pedidos ext. + Hall + motor | 9600 |
| [oled/](oled/) | TCA9548A + 4× SH1106 (versão completa) | 9600 |

## Upload rápido

```powershell
cd elevador/testes/bancada
.\upload.ps1 hall_diag
.\upload.ps1 hall_sequencia
.\upload.ps1 motor_hall_pedidos
.\upload.ps1 maquete          # firmware maquete completo
```

## Ordem recomendada

1. `pedidos_exteriores` — botões e LEDs
2. `hall_diag` — confirmar D7/D8/D11/D12
3. `hall_sequencia` — motor + sensores
4. `hall_porta` + `reset_d10` — porta e reset
5. `motor_hall_pedidos` — integração pedidos + movimento
6. **`maquete`** — [codigo_final_3_0_maquete/ext_porta](../../real/codigo_final_3_0_maquete/ext_porta/)

## Pinagem maquete

| Piso | Hall |
|------|------|
| 1 | D7 |
| 2 | D8 |
| 3 | D11 |
| 4 | D12 |

Porta: D22 · Reset: D10 · Ver [pinagem.md](../../documentacao/pinagem.md)

## Legacy

Testes obsoletos: [legacy/](legacy/)

## Período

Fevereiro – Julho 2026
