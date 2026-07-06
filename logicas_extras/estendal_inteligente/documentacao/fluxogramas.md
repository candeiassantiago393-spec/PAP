# Fluxogramas — Estendal Inteligente

## Estado

**Por fazer** — diagramas a criar.

---

## Fluxogramas planeados

| ID | Diagrama | Estado |
|----|----------|--------|
| F1 | Loop principal — sensor chuva → servo | Por fazer |
| F2 | Sequência recolher (0→90°) + pausa 8 s + reabrir | Por fazer |
| F3 | Integração futura com central de controlo | Por fazer |

---

## F1 — Lógica chuva (rascunho textual)

```text
[loop]
   ↓
[Ler sensor D2]
   ↓
  HIGH? ──Não──► [volta ao loop]
   │
  Sim
   ↓
[Servo 0→90° suave]
   ↓
[delay 8000 ms]
   ↓
[Servo 90→0° suave]
   ↓
[volta ao loop]
```

---

## Por fazer

- [ ] Desenhar F1 em Mermaid
- [ ] Desenhar F2 com tempos (50 ms/passo servo)
- [ ] Diagrama F3 — como avisar central se chove (futuro)
