# Compras e Justificação — PAP

Secção para **faturas**, **encomendas Mauser** e **justificação técnica** de cada componente.

## Estrutura

```text
docs/compras/
├── README.md                 ← este ficheiro (índice)
├── justificacao/             ← **componente + função** (ver README)
│   ├── README.md             ← índice geral
│   ├── elevador.md           ← E07/E08
│   └── logicas_extras.md     ← E12
└── faturas_mauser/           ← PDFs / imagens das faturas
    ├── _indice.md            ← tabela componente ↔ fatura ↔ etapa
    └── (faturas a adicionar)
```

## Ligação às etapas

| Etapa | Componentes típicos | Faturas |
|-------|---------------------|---------|
| [E07](../ETAPAS/relatorios/E07_testes_bancada.md) | Fonte 12 V, motor, L298N, OLEDs, Hall | _pendente_ |
| [E08](../ETAPAS/relatorios/E08_montagem_maquete.md) | Fixações, cablagem, estrutura | _pendente_ |
| [E00](../ETAPAS/relatorios/E00_planeamento.md) | Dimensionamento motor | [Dimensionamentos_motor.docx](../relatorios/Dimensionamentos_motor.docx) |

## Como será organizado

Quando enviares as faturas Mauser:

1. Cada fatura vai para `faturas_mauser/` com nome claro (ex.: `2026-02-15_mauser_fonte_12v.pdf`)
2. O ficheiro `_indice.md` liga **componente → fatura → etapa → preço**
3. Cada relatório de etapa (`E07`, `E08…`) terá secção **Compras relacionadas** com link directo
4. Justificações: cada componente com **função no sistema** em [justificacao/](justificacao/)

## Estado

- [x] 4 documentos Mauser organizados (3 faturas + 1 recibo loja) — ver [_indice.md](faturas_mauser/_indice.md)
- [x] Dimensionamento motor — [Dimensionamentos_motor.docx](../relatorios/Dimensionamentos_motor.docx)
- [x] Justificações por componente — [justificacao/](justificacao/)
- [ ] Mais faturas (motor, L298N, Hall, jumpers, ponteiras…)

## Fornecedor principal

**Mauser** — componentes electrónicos da maquete e bancada.
