# Interface — Organização de Trabalho

App desenvolvida pelo autor (Santiago Candeias) para gestão pessoal de projetos, tarefas e documentação técnica do PAP.

## Repositório

**[geral-controll-app](https://github.com/candeiassantiago393-spec/geral-controll-app)**

## Função na PAP

- Organizar fases do elevador (Displays, Sensores, Comissionamento, etc.)
- Registar BOM, notas técnicas e decisões de hardware
- Hub de trabalho durante desenvolvimento e apresentação

## Sync com este repo

Tasks exportadas da app ficam em [docs/TASKS_PAP_SYNC.md](../docs/TASKS_PAP_SYNC.md).

> **Este código não vive no repo PAP.** O firmware do painel ESP32 (antigo projeto `ecra`) está em [logicas_extras/paineis_controlo/](../logicas_extras/paineis_controlo/).

## Uso no Cursor

### Opção A — Pasta irmã no workspace
`File → Add Folder to Workspace` → clone de `geral-controll-app`

### Opção B — Servidor local
```bash
cd path/to/geral-controll-app
python -m http.server 8080
```
Abrir `http://localhost:8080` no Simple Browser do Cursor.

## Na apresentação

Referir como ferramenta de organização desenvolvida pelo autor — não faz parte do firmware do elevador, mas demonstra gestão profissional do projeto.

## Deploy

- GitHub Pages: `https://candeiassantiago393-spec.github.io/geral-controll-app/`
- Render: `https://candeias-app.onrender.com/` (se configurado)

## Navegação

| | |
|--|--|
| Índice repo | [../docs/INDICE_REPOSITORIO.md](../docs/INDICE_REPOSITORIO.md) |
| Tasks sync | [../docs/TASKS_PAP_SYNC.md](../docs/TASKS_PAP_SYNC.md) |
| Raiz PAP | [../README.md](../README.md) |
