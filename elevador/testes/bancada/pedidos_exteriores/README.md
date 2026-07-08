# Teste — Pedidos exteriores

Validação isolada de botões de chamada exterior e LEDs de indicação.

| Ficheiro | Descrição |
|----------|-----------|
| [src/main.ino](src/main.ino) | **Principal** — botão + LED com latch (PlatformIO) |
| [legacy/Test_01_External_Requests_Button_LED_Latched.ino](legacy/Test_01_External_Requests_Button_LED_Latched.ino) | Versão Arduino IDE (Test_01) |
| [legacy/Test_02_External_Buttons_Only.ino](legacy/Test_02_External_Buttons_Only.ino) | Apenas leitura de botões (Test_02) |

## Pinagem (Mega)

- Botões exteriores: D2–D5
- LEDs exteriores: A0–A3

## Upload

```bash
cd elevador/testes/bancada/pedidos_exteriores
pio run -t upload --upload-port COM8
pio device monitor   # 9600 baud
```

## Etapa

[E07 — Testes de bancada](../../../../docs/ETAPAS/relatorios/E07_testes_bancada.md)
