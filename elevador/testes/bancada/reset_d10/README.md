# Teste — botão reset D10 (módulo kit)

Valida o módulo de botão do kit (VCC, GND, SIG) antes de usar no elevador.

## Cablagem

| Módulo | Mega |
|--------|------|
| VCC | 5V |
| GND | GND |
| SIG | **D10** |

## Upload

```powershell
cd elevador/testes/bancada/reset_d10
pio run -t upload --upload-port COM5
pio device monitor
```

## O que deves ver

- Soltar: `D10=HIGH | SOLTO`
- Premir: `>> PREMIDO (LOW)` + LED D13 aceso

Se premido = HIGH, o módulo é active-high — avisa para inverter a lógica no firmware do elevador.
