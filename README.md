# PAP — Prédio Inteligente

**Curso:** Técnico de Eletrónica, Automação e Comando  
**Escola:** Escola Secundária Emídio Navarro  
**Ano letivo:** 2025/2026  
**Equipa:** Santiago Candeias · Arthur Buss · Rafael Lousada

---

## Visão do projeto

A PAP consiste no desenvolvimento de um **prédio inteligente** em escala de maquete, com o **elevador de 4 pisos** como subsistema principal e várias **lógicas extras** (segurança, meteorologia, RFID, porta do prédio, toldo automático, etc.).

Este repositório é o **ambiente de trabalho central** — código, simulações, testes, documentação, imagens da maquete e interface de organização, tudo acessível a partir do Cursor.

---

## Fase atual (junho 2026)

| Etapa | Estado |
|-------|--------|
| Simulação virtual (Tinkercad → Wokwi v01–v06) | Concluída |
| Testes de bancada (componentes isolados) | Concluída |
| Montagem na maquete física | **Em curso** |
| Integração motor + sensores Hall | Próximo passo |

**Já montado na maquete:** botões e LEDs de pedidos exteriores, displays OLED.  
**A seguir:** sensores de piso, motor de passo + L298N.

---

## Topologia do repositório

```text
PAP/
├── docs/                    → Contexto, dificuldades, relatórios, guia de apresentação
├── elevador/                → Sistema principal (simulação, real, testes, maquete, hardware)
├── logicas_extras/          → Subsistemas do prédio (versões iniciais, foco futuro)
├── interface/               → Referência à app de organização de trabalho
└── .vscode/                 → Configuração Cursor (PlatformIO, extensões)
```

### Documentação essencial

| Ficheiro | Conteúdo |
|----------|----------|
| [docs/CONTEXTO_PROJETO.md](docs/CONTEXTO_PROJETO.md) | Histórico, versões, pinagem, estados do sistema |
| [docs/ETAPAS/README.md](docs/ETAPAS/README.md) | **Etapas do projeto** — relatórios por fase |
| [docs/DIFICULDADES.md](docs/DIFICULDADES.md) | Registo de problemas e soluções |
| [docs/compras/](docs/compras/) | Faturas Mauser e justificação de compras |
| [docs/APRESENTACAO_CURSOR.md](docs/APRESENTACAO_CURSOR.md) | Como demonstrar tudo a partir do Cursor |
| [docs/relatorios/](docs/relatorios/) | PDFs e DOCX de especificação e relatórios |
| [elevador/maquete/ESTADO_ATUAL.md](elevador/maquete/ESTADO_ATUAL.md) | Progresso da montagem física |

---

## Desenvolvimento no Cursor

| Tarefa | Pasta | Comando / ação |
|--------|-------|----------------|
| Simular elevador (referência) | `elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/` | `F1` → *Wokwi: Start Simulator* |
| Compilar firmware real | `elevador/real/l298n_sh1106_hall/` | `pio run` |
| Upload para Mega | idem | `pio run -t upload` |

**PlatformIO validado** (22/06/2026): compilação OK — RAM 13,5%, Flash 8,4%.

Extensões recomendadas: Wokwi, PlatformIO IDE (Cursor).

---

## Repositórios relacionados

| Repo | Função |
|------|--------|
| [Elevador-Projeto-Final-Curso-PAP-](https://github.com/candeiassantiago393-spec/Elevador-Projeto-Final-Curso-PAP-) | Código legado — **migrado para este repo** |
| [geral-controll-app](https://github.com/candeiassantiago393-spec/geral-controll-app) | Interface de organização de trabalho (desenvolvida pelo autor) |

---

## Estrutura completa

Todas as pastas do repositório têm README e ligações — ver [elevador/README.md](elevador/README.md) e [logicas_extras/README.md](logicas_extras/README.md).

Histórico de migração: [docs/CONTEXTO_PROJETO.md](docs/CONTEXTO_PROJETO.md#plano-de-migração)
