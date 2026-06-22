# Lógicas Extras — Prédio Inteligente

Subsistemas do prédio inteligente **além do elevador**. Versões iniciais — foco principal continua no elevador até conclusão da maquete.

## Módulos

| Pasta | Função | Estado |
|-------|--------|--------|
| [cofre/](cofre/) | Cofre com código secreto (teclado + servo) | **Simulação Wokwi** |
| [porta_predio/](porta_predio/) | Código de acesso à entrada | Inicial |
| [seguranca/](seguranca/) | Alarmes e proteção | Inicial |
| [paineis_controlo/](paineis_controlo/) | Supervisão e comandos gerais | Inicial |
| [meteorologia/](meteorologia/) | Sensores ambientais | Inicial |
| [rfid/](rfid/) | Cartões por andar (prédio + apartamento) | Inicial |
| [toldo_automatico/](toldo_automatico/) | Estendal por sensor de chuva | Inicial |
| [legacy/](legacy/) | Versões antigas de cada módulo | — |

## Estrutura por módulo

```text
<nome_modulo>/
├── README.md
├── simulacao/
├── real/
├── testes/
└── documentacao/
```

## Prioridade

1. Concluir elevador na maquete
2. Migrar código inicial de cada extra para a pasta respetiva
3. Integrar na apresentação do prédio inteligente

## Etapa

[E12 — Lógicas extras](../docs/ETAPAS/relatorios/E12_logicas_extras.md)
