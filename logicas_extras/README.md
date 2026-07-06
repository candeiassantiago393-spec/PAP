# Lógicas Extras — Prédio Inteligente

Subsistemas do prédio inteligente **além do elevador**.

## Módulos

| Pasta | Função | Estado |
|-------|--------|--------|
| [cartoes_predio/](cartoes_predio/) | Cartões RFID — acesso por andar | Inicial |
| [central_de_controlo/](central_de_controlo/) | Caixa ESP32 OLED (ambiente, PIR, estufa, gás, fogo, sismo) | **Simulação Wokwi** |
| [cofre/](cofre/) | Código PIN porta prédio (teclado + servo) | **Simulação Wokwi** |
| [estendal_inteligente/](estendal_inteligente/) | Estendal por sensor de chuva | **Simulação Wokwi** |
| [legacy/](legacy/) | Versões antigas | — |

> `cofre/` será renomeado para `codigo_porta_predio` quando possível.

## Estrutura por módulo

```text
<nome_modulo>/
├── README.md           → visão geral do módulo
├── documentacao/       → funcionamento.md, pinagem.md
├── simulacao/          → wokwi/ quando aplicável
├── real/               → montagem física: pinagem.md, notas, fotos/, código PlatformIO
└── testes/             → registo de testes (preencher na bancada)
```

## Compilar / simular

| Módulo | Comando |
|--------|---------|
| Central de controlo | `cd central_de_controlo && pio run` → Wokwi |
| Código porta prédio | `cd cofre/simulacao/wokwi && pio run` → Wokwi |
| Estendal | `cd estendal_inteligente/simulacao/wokwi && pio run` → Wokwi |

## Prioridade

1. Concluir elevador na maquete
2. Montar lógicas extras na apresentação (17 julho)
3. Preencher `real/` e `testes/` à medida que se monta hardware

## Navegação

| | |
|--|--|
| Índice repo | [../docs/INDICE_REPOSITORIO.md](../docs/INDICE_REPOSITORIO.md) |
| Elevador | [../elevador/README.md](../elevador/README.md) |
| Etapa E12 | [../docs/ETAPAS/relatorios/E12_logicas_extras.md](../docs/ETAPAS/relatorios/E12_logicas_extras.md) |
| Raiz PAP | [../README.md](../README.md) |
