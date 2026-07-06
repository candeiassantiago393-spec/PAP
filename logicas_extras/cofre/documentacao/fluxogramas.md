# Fluxogramas — Código Porta Prédio

## Estado

**Por fazer** — diagramas a criar.

---

## Fluxogramas planeados

| ID | Diagrama | Estado |
|----|----------|--------|
| F1 | Fluxo teclado → validação PIN | Por fazer |
| F2 | Código correcto — abrir, esperar 8 s, fechar | Por fazer |
| F3 | Código incorrecto — feedback erro | Por fazer |

---

## F1 — Validação PIN (rascunho textual)

```text
[Tecla premida]
       ↓
  Numérica? ──Sim──► [input += tecla]
       │
  * ? ──Sim──► [input = ""]
       │
  # ? ──Sim──► [input == 1904?]
                    ├─ Sim → [Verde, abrir servo, 8s, fechar]
                    └─ Não → [Vermelho, bip erro]
              [input = ""]
```

---

## Por fazer

- [ ] Desenhar F1 completo com biblioteca Keypad
- [ ] Desenhar F2 com animação suave do servo (for loop)
- [ ] Adicionar diagrama ao relatório / apresentação 17 jul
