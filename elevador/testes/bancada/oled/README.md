# Teste — OLED / TCA9548A

Validação dos 4 displays OLED via multiplexador I2C.

| Ficheiro | Descrição |
|----------|-----------|
| [src/main.ino](src/main.ino) | Diagnóstico I2C — ping TCA + 4 canais + texto "OLED OK" |
| [legacy/oledbecnch.ino](legacy/oledbecnch.ino) | Versão anterior (Arduino IDE) |

## Hardware

- Arduino Mega — SDA D20, SCL D21
- TCA9548A (endereço `0x70`)
- 4× OLED SH1106 1,3" (canais 0–3, endereço `0x3C`)

## Upload

```bash
cd elevador/testes/bancada/oled
pio run -t upload --upload-port COM8
pio device monitor   # 9600 baud
```

## Nota

Este teste ajudou a identificar interferência I2C com o motor — resolvida no firmware real com refresh optimizado (ver E07).

## Etapa

[E07 — Testes de bancada](../../../../docs/ETAPAS/relatorios/E07_testes_bancada.md)

## Evidência

Foto bancada: [../imagens/bancada_integracao_fev_mar_2026.png](../imagens/bancada_integracao_fev_mar_2026.png)
