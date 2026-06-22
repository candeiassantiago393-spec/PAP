# Teste — OLED / TCA9548A

Validação dos 4 displays OLED via multiplexador I2C.

| Ficheiro | Descrição |
|----------|-----------|
| [oledbecnch.ino](oledbecnch.ino) | Teste de refresh nos 4 canais do TCA9548A |

## Hardware

- Arduino Mega
- TCA9548A (endereço `0x70`)
- 4× OLED SH1106 1,3" (canais 0–3)

## Nota

Este teste ajudou a identificar interferência I2C com o motor — resolvida no firmware real com refresh optimizado.

## Etapa

[E07 — Testes de bancada](../../../../docs/ETAPAS/relatorios/E07_testes_bancada.md)

## Evidência

Foto bancada: [../imagens/bancada_integracao_fev_mar_2026.png](../imagens/bancada_integracao_fev_mar_2026.png)
