# Implementação real — Cartões de Prédio

Montagem física do leitor RFID na porta do prédio / maquete.

## Estado

**Por fazer** — simulação ainda não existe; hardware RC522 comprado mas não montado.

---

## O que deve ficar nesta pasta

Quando montares na bancada / maquete, documenta **tudo** aqui:

```text
real/
├── README.md              ← este ficheiro (actualizar estado)
├── platformio.ini         ← config ESP32 + MFRC522
├── src/main.cpp           ← firmware real (copiar/adaptar da simulação)
├── pinagem.md             ← pinagem DEFINITIVA (pode diferir do planeado)
├── notas_montagem.md      ← passo a passo, problemas, soluções
└── fotos/                 ← fotos da ligação e da maquete
    ├── rc522_ligacao.jpg
    ├── cartoes_teste.jpg
    └── maquete_porta.jpg
```

---

## Por fazer

- [ ] Criar firmware real (`platformio.ini` + `src/main.cpp`)
- [ ] Ligar RC522 ao ESP32 e confirmar pinagem
- [ ] Gravar **pinagem definitiva** em `pinagem.md` (se diferir da [documentação](../documentacao/pinagem.md))
- [ ] Ler UIDs dos cartões físicos e preencher tabela em [funcionamento](../documentacao/funcionamento.md)
- [ ] Ligar relé ou servo à fechadura
- [ ] Testar cartão autorizado vs negado
- [ ] Tirar **fotos** da montagem → pasta `fotos/`
- [ ] Escrever **notas de montagem** (`notas_montagem.md`)
- [ ] Registar resultados em [../testes/README.md](../testes/README.md)

---

## Pinagem definitiva

> **Fica por fazer** — preencher `pinagem.md` após ligar o hardware na bancada.

Plano inicial: [../documentacao/pinagem.md](../documentacao/pinagem.md)

---

## Fotos

> **Fica por fazer** — criar pasta `fotos/` com imagens da montagem.

---

## Notas de montagem

> **Fica por fazer** — criar `notas_montagem.md` com:
> - Data da montagem
> - Componentes usados (refs Mauser)
> - Dificuldades encontradas
> - Ajustes feitos vs simulação

---

## Código real

> **Fica por fazer** — ainda não existe firmware nesta pasta.

| Ficheiro | Estado |
|----------|--------|
| `platformio.ini` | Por criar |
| `src/main.cpp` | Por criar |

Base conceptual: [../documentacao/funcionamento.md](../documentacao/funcionamento.md)
