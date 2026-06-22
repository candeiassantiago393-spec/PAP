# Tasks PAP — sync Candeias Control App

> Snapshot automático a partir do backup `2026-06-22` (14:37 UTC).  
> Fonte local: `me/backups/santiago-2026-06-22.json` (não versionar o JSON completo — dados pessoais).  
> Projeto na app: **Elevador e Prédio Inteligente**

## Etapas na app

Displays · Planeamento · Sensores · Botões · LEDs · Botões e LEDs · Testes · Documentação · Apresentação

---

## Concluídas (11)

| Etapa | Título |
|-------|--------|
| Displays | 1º Andar display |
| Displays | 2º Andar sensor |
| Displays | Displays — processo de implementação |
| Displays | URGÊNCIA MÁXIMA 3º piso display |
| Planeamento | Dimensionar motor |
| Planeamento | Preparar programa Cursor + simulação + revisão testes |
| Planeamento | Documentação de etapas no Cursor |
| Planeamento | Soluções para melhorar contactos no Arduino |
| Testes | Pasta com conjuntos de testes |
| Testes | Test bench PAP (OLEDs + sensores) |
| Documentação | Arthur — documento de apresentação |

---

## Pendentes / em curso (11)

| Prioridade | Etapa | Kanban | Due | Título |
|------------|-------|--------|-----|--------|
| **Agora** | Sensores | Today | 25/06 | **Início soldagem sensores** — 4 pisos, cabos à medida, teste cada um |
| **Agora** | Sensores | Waiting | 30/06 | Testes efectivos + montagem sensores na maquete |
| Alta | Displays | Waiting | 26/06 | Soldar topo maquete — contactos displays pisos 1 e 2 |
| Alta | Botões e LEDs | Waiting | 21/06 | Corrigir bug botão/LED 3º andar |
| Média | Planeamento | To do | 25/06 | Colocar imagens na pasta Cursor context |
| Compras | — | To do | 01/07 | Ventoinha 12V (driver L298N) |
| Compras | — | To do | 01/07 | Ventoinha para comprar (link Mauser) |
| — | — | Done* | 15/06 | Relatório dimensionamentos *(marcado done na app)* |
| — | — | Done* | 16/06 | Solução contactos Arduino |
| — | — | Waiting | 21/06 | Senha DGES *(não PAP técnico)* |
| — | — | To do | — | Ventoinha para driver elevador (ideia) |

\*Verificar estado real na app.

---

## Próximo foco (alinhado com E08)

1. Soldagem + teste Hall ×4 (TO-92S, íman na cabine)
2. Contactos displays topo maquete (pisos 1–2)
3. Motor + L298N + ventoinha 12V
4. Bug 3º andar botão/LED
5. Apresentação 17 julho

---

## Nota repo

URL do projeto na app ainda aponta para o repo legado — actualizar para:

`https://github.com/candeiassantiago393-spec/PAP.git`

---

## Como actualizar este ficheiro

1. App → Settings → Data backup → Export
2. Guardar em `me/backups/santiago-AAAA-MM-DD.json`
3. Pedir no Cursor: *"actualiza TASKS_PAP_SYNC a partir do backup"*
