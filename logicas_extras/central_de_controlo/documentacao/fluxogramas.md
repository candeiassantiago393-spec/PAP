# Fluxogramas — Central de Controlo

## Estado

**Por fazer** — diagramas a criar para documentação e apresentação.

---

## Fluxogramas planeados

| ID | Diagrama | Estado |
|----|----------|--------|
| F1 | Loop principal (`setup` + `loop`) | Por fazer |
| F2 | Navegação entre ecrãs (NEXT / modo feira) | Por fazer |
| F3 | Prioridade de alarmes (fogo → sismo → intruso → gás) | Por fazer |
| F4 | Lógica estufa (solo seco / ideal / molhado) | Por fazer |
| F5 | Deteção intruso PIR + silenciar com OK | Por fazer |

---

## F3 — Prioridade alarmes (rascunho textual)

```text
[Leitura sensores]
       ↓
  fireAlert? ──Sim──► [Ecrã FOGO / INCENDIO]
       │Não
  quakeAlert? ──Sim──► [Ecrã SISMO / TERRAMOTO]
       │Não
  alertActive? ──Sim──► [Ecrã ALERTA Intruso]
       │Não
  gasAlert? ──Sim──► [Ecrã FUGA DE GAS!!]
       │Não
  [Ecrã normal + dots]
```

---

## Por fazer

- [ ] Fluxograma F1 — arquitectura geral do firmware
- [ ] Fluxograma F2 — transições entre os 9 ecrãs
- [ ] Fluxograma F3 — árvore de prioridade de alarmes
- [ ] Fluxograma F4 — bomba + LEDs da estufa
- [ ] Exportar imagens para relatório PAP
