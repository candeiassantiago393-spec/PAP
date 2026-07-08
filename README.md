# PAP — Prédio Inteligente

**Curso:** Técnico de Eletrónica, Automação e Comando  
**Escola:** Escola Secundária Emídio Navarro  
**Ano letivo:** 2025/2026  
**Equipa:** Santiago Candeias · Arthur Buss · Rafael Lousada

---

## Visão do projeto

Protótipo de **prédio inteligente** em maquete: **elevador 4 pisos** (subsistema principal) + **lógicas extras** (código prédio, painel ESP32, RFID, porta, etc.).

Este repositório concentra código, simulações Wokwi, testes, documentação, esquemas e registo da maquete — ambiente de trabalho no **Cursor**.

---

## Navegação rápida

| Quero… | Ir para |
|--------|---------|
| **Mapa completo do repo** | [docs/INDICE_REPOSITORIO.md](docs/INDICE_REPOSITORIO.md) |
| Contexto e histórico | [docs/CONTEXTO_PROJETO.md](docs/CONTEXTO_PROJETO.md) |
| Etapas E00–E12 | [docs/ETAPAS/README.md](docs/ETAPAS/README.md) |
| Tasks / prioridades | [docs/TASKS_PAP_SYNC.md](docs/TASKS_PAP_SYNC.md) |
| Estado da maquete | [elevador/maquete/ESTADO_ATUAL.md](elevador/maquete/ESTADO_ATUAL.md) |
| Apresentação 17 jul | [docs/APRESENTACAO_CURSOR.md](docs/APRESENTACAO_CURSOR.md) |

---

## Fase atual (julho 2026)

| Etapa | Estado |
|-------|--------|
| Simulação Wokwi v01–v06 | Concluída |
| Testes bancada | Concluída |
| **Montagem maquete (E08)** | **Em curso** — botões novos a integrar e testar |
| Hall + motor + L298N | Próximo |
| Lógicas extras | Programadas (virtual + componentes em casa); montagem na maquete esta semana |
| Decoração maquete | **Sábado 11 jul** — papel autocolante, bonecos e miniaturas interiores |

**Montado:** botões/LEDs exteriores, OLEDs, estrutura madeira + cabine.  
**A seguir:** testes lógica elevador, sensores Hall, motor, montagem extras, acabamento visual.

---

## Topologia

```text
PAP/
├── docs/              → Índice, etapas, compras, relatórios
├── elevador/          → Simulação, real, testes, maquete, hardware
├── logicas_extras/    → Cofre, painel ecra, RFID, meteorologia…
├── interface/         → App geral-controll-app
└── .vscode/           → PlatformIO + Wokwi
```

| Área | README |
|------|--------|
| Elevador | [elevador/README.md](elevador/README.md) |
| Lógicas extras | [logicas_extras/README.md](logicas_extras/README.md) |
| Documentação | [docs/README.md](docs/README.md) |

---

## Desenvolvimento no Cursor

| Tarefa | Pasta | Ação |
|--------|-------|------|
| Simular elevador v06 | `elevador/simulacao/legacy/wokwi/v06_.../wokwi/` | Wokwi: Start Simulator |
| Firmware Mega (maquete) | `elevador/real/l298n_sh1106_hall/` | `pio run` |
| Código prédio PIN | `logicas_extras/codigo_predio/simulacao/wokwi/` | Wokwi |
| Painel ESP32 ecra | `logicas_extras/paineis_controlo/real/caixa_ecra/` | `pio run` → simular em `.../simulacao/wokwi/` |

Extensões: **PlatformIO IDE**, **Wokwi Simulator**.

---

## Repositórios relacionados

| Repo | Função |
|------|--------|
| [PAP](https://github.com/candeiassantiago393-spec/PAP.git) | **Este repositório** |
| [geral-controll-app](https://github.com/candeiassantiago393-spec/geral-controll-app) | Organização de tarefas |
| [Elevador legado](https://github.com/candeiassantiago393-spec/Elevador-Projeto-Final-Curso-PAP-) | Migrado para aqui |

Histórico de migração: [docs/CONTEXTO_PROJETO.md](docs/CONTEXTO_PROJETO.md#plano-de-migração)

---

## O que está neste repo (e o que não está)

| Projeto local / nome | No PAP? | Onde |
|----------------------|---------|------|
| **ecra** (painel ESP32) | **Sim** — código migrado | [logicas_extras/paineis_controlo/](logicas_extras/paineis_controlo/) |
| **me** (Candeias Control App) | **Sim** | [interface/](interface/) |
| Tasks da app | Snapshot apenas | [docs/TASKS_PAP_SYNC.md](docs/TASKS_PAP_SYNC.md) |

> Um `git clone` deste repo traz elevador, código prédio, painel ecra, app de organização (`interface/`) e documentação.
