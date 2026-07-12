# Código Final 3.0 — versão completa (ARQUIVO CONGELADO)

> **Não alterar este firmware.** Referência histórica da versão totalmente completa do elevador.

## Conteúdo

| Funcionalidade | Incluído |
|----------------|----------|
| Motor L298N (4 fios) | Sim |
| 4× OLED SH1106 via TCA9548A | Sim |
| Pedidos exteriores (D2–D5, LEDs A0–A3) | Sim |
| Pedidos interiores (D23–D26, LEDs D27–D30) | Sim |
| Sensores Hall pisos (D7, D8, D11, D12) | Sim |
| Buzzer, reset/rearme (D10) | Sim |
| Permissiva porta D22 | **Não** (ver variant maquete) |

## Pinagem

Ver [pinagem.md](../../documentacao/pinagem.md).

## Compilar / upload

```powershell
cd elevador/real/codigo_final_3_0
pio run -t upload
pio device monitor -b 115200
```

## Variante activa na maquete

Para a maquete **sem OLED**, **sem botões interiores**, **com porta D22**:

→ [codigo_final_3_0_maquete/](../codigo_final_3_0_maquete/)

## Nota sobre origem

O ficheiro `Codigo_Final_3_0.ino` no Desktop estava vazio no momento do arquivamento.  
Esta cópia provém de `l298n_sh1106_hall` (versão completa do repositório).  
Se tiveres o `.ino` original completo, repõe-o em `src/main.ino` **sem alterar a lógica**.

## Navegação

| | |
|--|--|
| Real | [../README.md](../README.md) |
| Simulação v06 | [../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/) |
