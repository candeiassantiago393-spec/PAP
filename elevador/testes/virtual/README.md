# Testes virtuais — Elevador

Testes de lógica **sem hardware** — validação de algoritmos isolados do elevador.

## Estado

**Fase futura.** A lógica actual é validada em:

- Simulação Wokwi v01–v06
- Testes de bancada com hardware real

## Ideias para esta pasta

| Teste | Descrição |
|-------|-----------|
| Fila de pedidos | Unit test da lógica `pedidos[]` e priorização |
| Máquina de estados | Transições NORMAL → PORTA → ERRO sem Arduino |
| Debounce porta | Simulação temporal de abertura/fecho |

## Estrutura sugerida (quando implementar)

```text
virtual/
├── README.md
├── fila_pedidos/
└── estados/
```

## Referência de lógica

Código de referência: `simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/sketch.ino`
