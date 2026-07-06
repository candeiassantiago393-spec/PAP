# Implementação real — Estendal Inteligente

Montagem do sensor de chuva no topo do prédio e servo no estendal.

## Estado

**Por fazer** — sensor Temu comprado; simulação Wokwi OK; montagem na maquete pendente.

---

## O que deve ficar nesta pasta

```text
real/
├── README.md
├── platformio.ini
├── src/main.cpp           ← firmware real
├── pinagem.md             ← pinagem DEFINITIVA sensor Temu + servo
├── notas_montagem.md
└── fotos/
    ├── sensor_topo_predio.jpg
    ├── servo_estendal.jpg
    └── teste_chuva.jpg
```

---

## Por fazer

- [ ] Instalar sensor de chuva Temu no **topo da maquete**
- [ ] Confirmar lógica do sensor (HIGH ou LOW quando chove?)
- [ ] Confirmar tensão (3.3 V vs 5 V)
- [ ] Montar servo no mecanismo do estendal
- [ ] Escolher MCU (Arduino Uno dedicado vs ESP32 partilhado)
- [ ] Copiar firmware de [../simulacao/wokwi/sketch.ino](../simulacao/wokwi/sketch.ino)
- [ ] Gravar **pinagem definitiva** em `pinagem.md`
- [ ] Testar com regador (água) vs seco
- [ ] Ajustar ângulos servo à mecânica real
- [ ] Tirar **fotos** → `fotos/`
- [ ] Escrever **notas de montagem**
- [ ] Registar testes em [../testes/README.md](../testes/README.md)

---

## Pinagem definitiva

> **Fica por fazer** — sensor Temu pode ter pinagem DO/AO diferente do slide switch Wokwi.

Plano simulação: [../documentacao/pinagem.md](../documentacao/pinagem.md)

---

## Fotos

> **Fica por fazer** — sensor no topo, servo no estendal, teste com água.

---

## Notas de montagem

> **Fica por fazer** — documentar:
> - Modelo exacto do sensor Temu
> - Fixação no topo do prédio (cola, parafuso, suporte)
> - Ligação mecânica servo ↔ estendal
> - Se mantém lógica de 8 s aberto/fechado ou ajustar
> - Falsos positivos (condensação, pingos)

---

## Código real

| Ficheiro | Estado | Origem |
|----------|--------|--------|
| `../simulacao/wokwi/sketch.ino` | ✓ Simulação | Wokwi validado |
| `real/platformio.ini` | Por criar | Copiar de simulação |
| `real/src/main.cpp` | Por criar | Adaptar sketch.ino |

Comando simulação: `cd ../simulacao/wokwi && pio run`
