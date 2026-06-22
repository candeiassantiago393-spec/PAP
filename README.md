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
| [docs/DIFICULDADES.md](docs/DIFICULDADES.md) | Registo de problemas e soluções |
| [docs/APRESENTACAO_CURSOR.md](docs/APRESENTACAO_CURSOR.md) | Como demonstrar tudo a partir do Cursor |
| [elevador/maquete/ESTADO_ATUAL.md](elevador/maquete/ESTADO_ATUAL.md) | Progresso da montagem física |

---

## Repositórios relacionados

| Repo | Função |
|------|--------|
| [Elevador-Projeto-Final-Curso-PAP-](https://github.com/candeiassantiago393-spec/Elevador-Projeto-Final-Curso-PAP-) | Código legado (a migrar para aqui) |
| [geral-controll-app](https://github.com/candeiassantiago393-spec/geral-controll-app) | Interface de organização de trabalho (desenvolvida pelo autor) |

---

## Migração em curso

O conteúdo do repositório antigo está a ser reorganizado para esta estrutura.  
Commits planeados por etapas — ver [docs/CONTEXTO_PROJETO.md](docs/CONTEXTO_PROJETO.md#plano-de-migração).
