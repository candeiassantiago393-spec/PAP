# Lógicas Extras — Prédio Inteligente

Subsistemas do prédio inteligente **além do elevador**. Versões iniciais — foco principal continua no elevador até conclusão da maquete.

## Módulos

| Pasta | Função | Estado |
|-------|--------|--------|
| [cofre/](cofre/) | Cofre com código secreto (teclado + servo) | **Simulação Wokwi** |
| [paineis_controlo/](paineis_controlo/) | Caixa ESP32 OLED multi-ecrã (meteorologia, PIR) | **Simulação Wokwi + real** |
| [porta_predio/](porta_predio/) | Código de acesso à entrada | Inicial |
| [seguranca/](seguranca/) | Alarmes e proteção (PIR no painel ecra) | Inicial |
| [meteorologia/](meteorologia/) | Sensores ambientais — **ecrãs AMBIENTE/GRÁFICO no painel ecra** | **Funcional (demo)** |
| [rfid/](rfid/) | Cartões por andar (prédio + apartamento) | Inicial |
| [toldo_automatico/](toldo_automatico/) | Estendal por sensor de chuva | Planeado |
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
| Painel ecra ESP32 | `cd paineis_controlo/real/caixa_ecra && pio run` |
| Cofre | Wokwi em `cofre/simulacao/wokwi/` |

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
