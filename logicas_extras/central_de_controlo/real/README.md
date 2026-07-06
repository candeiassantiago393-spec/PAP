# Implementação real — Central de Controlo

Montagem física da caixa ESP32 (estilo MagSafe) na maquete do prédio.

## Estado

**Por fazer** — simulação Wokwi funcional; caixa física e sensores reais por montar.

---

## O que deve ficar nesta pasta

```text
real/
├── README.md              ← este ficheiro
├── platformio.ini         ← config ESP32 (pode copiar da raiz do módulo)
├── src/main.cpp           ← firmware real (ajustes de pinagem vs simulação)
├── pinagem.md             ← pinagem DEFINITIVA da caixa montada
├── notas_montagem.md      ← passo a passo da caixa
└── fotos/
    ├── caixa_interior.jpg
    ├── oled_botoes.jpg
    └── sensores_ligados.jpg
```

> O firmware principal pode continuar na raiz (`../sketch.ino` + `../src/main.cpp`). Esta pasta guarda a **versão real** com pinagem e sensores físicos confirmados.

---

## Por fazer

- [ ] Montar ESP32 + OLED SSD1306 na caixa
- [ ] Ligar DHT22, PIR, buzzer, 3× LED, bomba/relé
- [ ] Substituir potenciómetros da simulação por sensores reais:
  - [ ] Solo → sensor capacitivo
  - [ ] Gás → MQ-2 ou MQ-135
  - [ ] Fogo → KY-026 (saída analógica AO)
  - [ ] Sismo → SW-420 (digital → converter pulsos em intensidade)
- [ ] Botões NEXT e OK no painel frontal
- [ ] Alimentação (USB ou 5 V regulado)
- [ ] Gravar **pinagem definitiva** em `pinagem.md`
- [ ] Copiar/ajustar firmware para `real/src/main.cpp` se pinagem mudar
- [ ] Tirar **fotos** → `fotos/`
- [ ] Escrever **notas de montagem**
- [ ] Registar testes em [../testes/README.md](../testes/README.md)

---

## Pinagem definitiva

> **Fica por fazer** — confirmar GPIO reais após montagem e gravar em `pinagem.md`.

Referência simulação: [../documentacao/pinagem.md](../documentacao/pinagem.md)

---

## Fotos

> **Fica por fazer** — documentar visualmente a caixa montada.

---

## Notas de montagem

> **Fica por fazer** — incluir:
> - Layout dos componentes dentro da caixa
> - Sensores Mauser usados (refs do kit 096-6786)
> - Diferenças face ao Wokwi (potenciómetros → sensores reais)
> - Calibração dos limites (50 % gás/fogo/sismo)

---

## Código real

| Ficheiro | Estado | Notas |
|----------|--------|-------|
| `../sketch.ino` + `../src/main.cpp` | ✓ Simulação | Manter em sync |
| `real/platformio.ini` | Por criar | Se pinagem real diferir |
| `real/src/main.cpp` | Por criar | Fork com `#define` de pinos reais |

Base: copiar de [../src/main.cpp](../src/main.cpp) quando montar.
