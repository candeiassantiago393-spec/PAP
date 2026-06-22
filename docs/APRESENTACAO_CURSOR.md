# Guia de Apresentação — Tudo a partir do Cursor

> Objetivo: na defesa da PAP, demonstrar código, simulação, documentação, maquete e organização **sem sair do Cursor**.

---

## Visão geral do fluxo

```text
┌─────────────────────────────────────────────────────────────┐
│                        CURSOR IDE                           │
├─────────────┬──────────────┬──────────────┬─────────────────┤
│  Explorer   │   Editor     │  Terminal    │  Simple Browser │
│  (topologia)│  (.ino/PDF)  │  (servidor,  │  (interface web,│
│             │              │   PlatformIO)│   Wokwi local)  │
└─────────────┴──────────────┴──────────────┴─────────────────┘
```

---

## 1. Abrir o workspace

1. `File → Open Folder` → pasta `PAP/`
2. Opcional: `File → Add Folder to Workspace` para incluir `geral-controll-app` como pasta irmã

**O que mostrar:** árvore de pastas segmentada (`elevador/`, `logicas_extras/`, `docs/`).

---

## 2. Documentação e relatório

| O quê | Onde abrir no Cursor |
|-------|----------------------|
| Contexto do projeto | `docs/CONTEXTO_PROJETO.md` |
| Especificação técnica | `docs/relatorios/` (PDF) |
| Dificuldades encontradas | `docs/DIFICULDADES.md` |
| Estado da maquete | `elevador/maquete/ESTADO_ATUAL.md` |

**Dica:** PDF abre no preview do Cursor. Markdown com preview lado a lado (`Ctrl+Shift+V`).

---

## 3. Evolução do código (legacy → atual)

Percorrer em ordem no Explorer:

```text
elevador/simulacao/legacy/tinkercad/v01_... → v02_...
elevador/simulacao/legacy/wokwi/v03_... → v06_atual_...
elevador/real/l298n_sh1106_hall/
```

**Narrativa sugerida:**
1. v01 — prova de conceito com motor DC
2. v02 — fila de pedidos
3. v03 — salto para Mega + stepper
4. v04–v05 — segurança e OLEDs
5. v06 — sistema completo simulado
6. real — adaptação L298N + Hall

---

## 4. Simulação Wokwi no Cursor

### Estado atual
Links Wokwi pendentes (export de casa). Quando disponíveis:

1. Colocar `diagram.json` + `wokwi.toml` em cada pasta legacy
2. Instalar extensão **Wokwi Simulator** no Cursor
3. Abrir pasta da versão → `F1` → "Wokwi: Start Simulator"

### Alternativa durante apresentação
- **Simple Browser** (`Ctrl+Shift+P` → "Simple Browser: Show") com URL do projeto Wokwi v06

---

## 5. Compilação e erros (PlatformIO)

Quando configurado:

```bash
# Na pasta do firmware ativo
pio run
pio run -t upload
```

**O que mostrar:** painel de problemas do Cursor com erros de compilação resolvidos em tempo real.

---

## 6. Interface de organização

App desenvolvida pelo autor para gestão do projeto PAP.

```bash
# Terminal integrado — servidor local da interface
cd interface   # ou pasta do geral-controll-app
python -m http.server 8080
```

Abrir no **Simple Browser:** `http://localhost:8080`

**Narrativa:** ferramenta pessoal de organização — tarefas, BOM, notas técnicas, fases do elevador.

---

## 7. Vídeo e fotos da maquete

| Recurso | Caminho |
|---------|---------|
| Vídeo montagem (jun 2026) | `elevador/maquete/imagens/videos/2026-06-22_maquete_botoes_leds_displays.mp4` |
| Fotos futuras | `elevador/maquete/imagens/fotos/` |

Reproduzir vídeo no leitor do sistema ou embutir na apresentação 3D (fase posterior).

---

## 8. Apresentação 3D (fase posterior)

Plano: experiência estilo Prezi para explorar componentes da maquete.

- Abrir pelo terminal: `npm run dev` (quando criada)
- Integrar no mesmo workspace Cursor
- Links para ficheiros de código e simulação a partir de hotspots 3D

---

## 9. Checklist pré-apresentação

- [ ] Workspace `PAP` abre sem erros
- [ ] PDF do relatório em `docs/relatorios/`
- [ ] Links Wokwi em cada README legacy
- [ ] PlatformIO compila firmware v06 ou real
- [ ] Interface local arranca com um comando
- [ ] Vídeo/fotos da maquete na pasta `maquete/imagens/`
- [ ] Apresentação 3D (quando pronta)

---

## 10. Ordem sugerida de demo (5–10 min)

1. **30 s** — README raiz: visão prédio inteligente
2. **1 min** — Explorer: topologia `elevador/` vs `logicas_extras/`
3. **2 min** — Percorrer legacy v01→v06 (código + README)
4. **2 min** — Wokwi v06 a correr (extensão ou browser)
5. **1 min** — `DIFICULDADES.md` — problemas reais resolvidos
6. **1 min** — Vídeo maquete + `ESTADO_ATUAL.md`
7. **1 min** — Interface organização no Simple Browser
8. **30 s** — Próximos passos: sensores, motor, 3D
