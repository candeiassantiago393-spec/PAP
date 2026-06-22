# Toldo Automático — Prédio Inteligente

Estendal que se fecha automaticamente quando chove.

## Objectivo na maquete

Sensor de chuva no **topo do prédio** deteta precipitação → aciona toldo/estendal na zona exterior da maquete.

## Estado (jun 2026)

| Item | Estado |
|------|--------|
| Conceito | Definido |
| Montagem | **Em curso** |
| Sensor de chuva | Comprado na **Temu** — por instalar no topo |
| Actuador | Por definir (servo/relé) |
| Firmware | Pendente — **após elevador na maquete** |

## Hardware planeado

| Componente | Estado |
|------------|--------|
| Sensor chuva (Temu) | Comprado — montagem no topo |
| Actuador toldo | Por definir |
| Microcontrolador | ESP32 ou partilhado com outro módulo |

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/](simulacao/) | Lógica chuva → acção |
| [real/](real/) | Firmware + sensor |
| [testes/](testes/) | Teste molhado/seco |
| [documentacao/](documentacao/) | Esquema de ligação |

## Relacionado

| Módulo | Ligação |
|--------|---------|
| Meteorologia | [meteorologia/](../meteorologia/) — DHT22 no painel ecra (ambiente interior) |

## Prioridade

Depois do **elevador funcional** na maquete (E08/E10).

## Navegação

| | |
|--|--|
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
| Etapa E12 | [../../docs/ETAPAS/relatorios/E12_logicas_extras.md](../../docs/ETAPAS/relatorios/E12_logicas_extras.md) |
