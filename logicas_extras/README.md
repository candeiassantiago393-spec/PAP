# Lógicas Extras — Prédio Inteligente

Subsistemas do prédio inteligente **além do elevador**. Versões iniciais — foco principal continua no elevador até conclusão da maquete.

## Módulos

| Pasta | Função | Estado |
|-------|--------|--------|
| [cartoes_predio/](cartoes_predio/) | Lógica de cartões de prédio (RFID por andar) | Inicial |
| [central_de_controlo/](central_de_controlo/) | Central de controlo ESP32 OLED multi-ecrã (ambiente, PIR, estufa, gás, fogo) | **Simulação Wokwi + real** |
| [cofre/](cofre/) | Código de acesso à porta do prédio (teclado + servo) — _rename para `codigo_porta_predio` pendente_ | **Simulação Wokwi** |
| [estendal_inteligente/](estendal_inteligente/) | Estendal por sensor de chuva | **Simulação Wokwi** |
| [legacy/](legacy/) | Versões antigas de cada módulo | — |

## Estrutura por módulo

```text
<nome_modulo>/
├── README.md
├── simulacao/     → wokwi/ quando aplicável
├── real/          → PlatformIO ou .ino
├── testes/
└── documentacao/  → funcionamento.md, pinagem.md
```

## Compilar no Cursor

| Módulo | Comando |
|--------|---------|
| Central de controlo ESP32 | `cd central_de_controlo && pio run` |
| Código porta prédio | `cd cofre/simulacao/wokwi && pio run` → Wokwi |

## Prioridade

1. Concluir elevador na maquete
2. Migrar código inicial de cada extra para a pasta respetiva
3. Integrar na apresentação do prédio inteligente (17 julho)

## Navegação

| | |
|--|--|
| Índice repo | [../docs/INDICE_REPOSITORIO.md](../docs/INDICE_REPOSITORIO.md) |
| Elevador | [../elevador/README.md](../elevador/README.md) |
| Tasks | [../docs/TASKS_PAP_SYNC.md](../docs/TASKS_PAP_SYNC.md) |
| Etapa E12 | [../docs/ETAPAS/relatorios/E12_logicas_extras.md](../docs/ETAPAS/relatorios/E12_logicas_extras.md) |
| Raiz PAP | [../README.md](../README.md) |
