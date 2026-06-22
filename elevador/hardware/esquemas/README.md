# Esquemas elétricos — Elevador

Esquemas formais de **força + comando** no formato de automatismos industriais.

## Desenhos principais

| ID | Ficheiro | Variante | Uso |
|----|----------|----------|-----|
| **ELV-SCH-001** | [elevador_v06_forca_comando.svg](elevador_v06_forca_comando.svg) | Wokwi v06 (A4988) | Simulação · relatório |
| **ELV-SCH-002** | [elevador_real_forca_comando.svg](elevador_real_forca_comando.svg) | Maquete (L298N + Hall) | Montagem E08 · E09 |

Documentação completa: [elevador_v06_esquema.md](elevador_v06_esquema.md)

## Conteúdo de cada desenho

- Cabeçalho com identificação do projecto
- Circuito de força: alimentação, fusível, driver, motor, I2C
- Circuito de comando: entradas, saídas, mapa de pinos
- Carimbo técnico (n.º desenho, revisão, data)
- Legenda de condutores por cor
- Notas de montagem e diferenças sim ↔ real

## Referências Wokwi

| Ficheiro | Descrição |
|----------|-----------|
| [esquematico_v06_wokwi.svg](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/esquematico_v06_wokwi.svg) | Ligação fio-a-fio (export) |
| [diagram.json](../../simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/diagram.json) | Componentes Wokwi |

## Pinagem

[documentacao/pinagem.md](../../documentacao/pinagem.md)

## Pendências

- [ ] Export PDF para relatório final PAP
- [ ] Esquema detalhado Fritzing/KiCad (resistências, bornes)
