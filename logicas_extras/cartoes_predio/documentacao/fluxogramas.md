# Fluxogramas — Cartões de Prédio

## Estado

**Por fazer** — diagramas a criar antes da apresentação / relatório.

---

## Fluxogramas planeados

| ID | Diagrama | Estado |
|----|----------|--------|
| F1 | Fluxo principal — cartão → UID → autorização → fechadura | Por fazer |
| F2 | Cartão autorizado vs negado | Por fazer |
| F3 | Níveis de acesso (hall / andar / visitante) | Por fazer |

---

## F1 — Fluxo principal (rascunho textual)

> Substituir por diagrama Mermaid ou imagem quando implementado.

```text
[Cartão aproximado] → [RC522 lê UID] → [UID na lista?]
                                              ├─ Sim → [Abrir fechadura X s]
                                              └─ Não → [LED/buzzer erro]
```

---

## Por fazer

- [ ] Desenhar fluxograma F1 (Mermaid ou draw.io)
- [ ] Desenhar fluxograma F2 (autorizado vs negado)
- [ ] Exportar PNG/SVG para o relatório PAP
- [ ] Ligar a [funcionamento.md](funcionamento.md)
