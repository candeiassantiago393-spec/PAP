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

### Onde estão os ficheiros

```text
elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/
```

> `simulacao/wokwi/exports/` está vazia por intenção — exports activos em `legacy/wokwi/v03–v06/wokwi/`.

### Passos

1. Instalar extensão **Wokwi Simulator** no Cursor
2. Abrir pasta `.../v06_.../wokwi/` (expandir `legacy` no Explorer)
3. Abrir `sketch.ino`
4. `F1` → **Wokwi: Start Simulator**

### Alternativa durante apresentação

**Simple Browser** com URL: https://wokwi.com/projects/464344193596402689

### Lógicas extras (Wokwi)

| Módulo | Pasta | Compilar antes (ESP32) |
|--------|-------|------------------------|
| Código prédio PIN | `logicas_extras/codigo_predio/simulacao/wokwi/` | — (sketch directo) |
| Painel ecra ESP32 | `logicas_extras/paineis_controlo/simulacao/wokwi/` | `pio run` em `.../real/caixa_ecra/` |

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
| OLEDs — teste 4 displays | `elevador/maquete/imagens/videos/2026-06-22_maquete_oled_displays_teste.mp4` |
| Botões + LEDs exteriores | `elevador/maquete/imagens/videos/2026-06-22_maquete_botoes_leds_exteriores.mp4` |
| Guias cabos (argolas) | `elevador/maquete/imagens/fotos/2026-06-22_guias_cabos_argolas.png` |
| Motor + tambor | `elevador/hardware/mecanica/imagens/2026-06-22_motor_nema17_tambor_correia.png` |
| Estado escrito | `elevador/maquete/ESTADO_ATUAL.md` |

Reproduzir vídeo no leitor do sistema ou embutir na apresentação 3D.

---

## Plano de apresentação (definido jun 2026)

Duração flexível (até ~3 h se necessário). Blocos previstos:

| # | Bloco | Onde no Cursor / repo |
|---|-------|------------------------|
| 1 | **Apresentação 3D** (estilo Prezi) | A desenvolver no workspace |
| 2 | **Especificação e relatório** | `docs/relatorios/` |
| 3 | **Interface de organização** | [geral-controll-app](https://github.com/candeiassantiago393-spec/geral-controll-app) — Simple Browser |
| 4 | **Simulador virtual v06** + resumo v01–v05 | Wokwi `v06_.../wokwi/` |
| 5 | **Funções físicas da maquete** | Vídeos + demo ao vivo (I/O montado; motor/Hall quando prontos) |
| 6 | **Lógicas extras** | Após elevador na maquete; até lá Wokwi (codigo_predio, ecra) se houver tempo |

**Nota:** Porta e botões interiores podem ficar **só na simulação** se não estiverem montados a tempo.

**Prioridade até 17 jul:** fechar elevador (Hall + motor) → depois extras (toldo no topo, RFID, etc.).

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
- [x] Wokwi lógicas extras: codigo_predio + painel ecra em `logicas_extras/`
- [ ] PlatformIO compila firmware v06 ou real
- [ ] Interface local arranca com um comando
- [x] Vídeo/fotos da maquete na pasta `maquete/imagens/`
- [ ] Apresentação 3D (quando pronta)

---

## 10. Ordem sugerida de demo (5–10 min)

1. **30 s** — README raiz: visão prédio inteligente
2. **1 min** — Explorer: topologia `elevador/` vs `logicas_extras/`
3. **2 min** — Percorrer legacy v01→v06 (código + README)
4. **2 min** — Wokwi v06 elevador + codigo_predio + painel ecra (extensão)
5. **1 min** — `DIFICULDADES.md` — problemas reais resolvidos
6. **1 min** — Vídeo maquete + `ESTADO_ATUAL.md`
7. **1 min** — Interface organização no Simple Browser
8. **30 s** — Próximos passos: sensores, motor, 3D
