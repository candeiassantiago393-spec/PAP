# Implementação real — Código Porta Prédio

Montagem do teclado + servo na porta do prédio na maquete.

## Estado

**Por fazer** — simulação Wokwi funcional; teclado Mauser e servo físicos por montar.

---

## O que deve ficar nesta pasta

```text
real/
├── README.md
├── platformio.ini         ← Arduino Uno ou ESP32
├── src/main.cpp           ← firmware real (adaptado da simulação)
├── pinagem.md             ← pinagem DEFINITIVA do teclado físico
├── notas_montagem.md
└── fotos/
    ├── teclado_mauser.jpg
    ├── servo_fechadura.jpg
    └── ligacao_arduino.jpg
```

---

## Por fazer

- [ ] Escolher MCU final (Arduino Uno vs ESP32)
- [ ] Ligar teclado matricial 3×4 ([096-4642](../../../docs/compras/faturas_mauser/2026EC1403323.md))
- [ ] Confirmar se pinagem do teclado real = simulação (filas/colunas)
- [ ] Montar servo na fechadura da porta
- [ ] Ligar LEDs vermelho/verde + buzzer (220 Ω nos LEDs)
- [ ] Copiar firmware de [../simulacao/wokwi/sketch.ino](../simulacao/wokwi/sketch.ino) → `src/main.cpp`
- [ ] Gravar **pinagem definitiva** em `pinagem.md`
- [ ] Testar código `1904#` na bancada
- [ ] Integrar na maquete do prédio
- [ ] Tirar **fotos** → `fotos/`
- [ ] Escrever **notas de montagem**
- [ ] Registar testes em [../testes/README.md](../testes/README.md)

---

## Pinagem definitiva

> **Fica por fazer** — o teclado Mauser pode ter ordem de pinos diferente do membrane keypad Wokwi.

Plano simulação: [../documentacao/pinagem.md](../documentacao/pinagem.md)

---

## Fotos

> **Fica por fazer** — foto do teclado ligado, servo na fechadura, montagem na maquete.

---

## Notas de montagem

> **Fica por fazer** — documentar:
> - Qual MCU foi usado
> - Mapeamento real filas/colunas do teclado Mauser
> - Ângulos do servo (0° fechado / 90° aberto) na mecânica real
> - Tempo de abertura (8 s) — confirmar se mantém

---

## Código real

| Ficheiro | Estado | Origem |
|----------|--------|--------|
| `../simulacao/wokwi/sketch.ino` | ✓ Simulação | Wokwi validado |
| `real/platformio.ini` | Por criar | Copiar de simulação |
| `real/src/main.cpp` | Por criar | Adaptar sketch.ino |

Comando simulação: `cd ../simulacao/wokwi && pio run`
