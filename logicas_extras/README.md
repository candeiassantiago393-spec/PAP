# Lógicas Extras — Prédio Inteligente

Subsistemas do prédio inteligente **além do elevador**. Versões iniciais — foco principal continua no elevador até conclusão da maquete.

## Módulos

| Pasta | Função | Estado |
|-------|--------|--------|
| [cofre/](cofre/) | Cofre com código secreto (teclado + servo) | **Simulação Wokwi** |
| [esp32-candeias-box/](esp32-candeias-box/) | Caixa ESP32 OLED multi-ecrã (ambiente, PIR, estufa, gás, fogo) | **Simulação Wokwi + real** |
| [rfid/](rfid/) | Cartões por andar (prédio + apartamento) | Inicial |
| [toldo_automatico/](toldo_automatico/) | Estendal por sensor de chuva | **Simulação Wokwi** |
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
| Painel ecra ESP32 | `cd esp32-candeias-box && pio run` |
| Cofre | `cd cofre/simulacao/wokwi && pio run` → Wokwi |

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
