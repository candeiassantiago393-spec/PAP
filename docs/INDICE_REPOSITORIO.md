# Índice do Repositório — PAP

> Mapa navegável de **todas** as pastas principais. Cada secção liga ao README local.

---

## Visão geral

```text
PAP/
├── docs/                 Documentação, etapas, compras, relatórios
├── elevador/             Sistema principal — 4 pisos, Mega, maquete
├── logicas_extras/       Subsistemas do prédio inteligente
├── interface/            App de organização (geral-controll-app)
└── .vscode/              PlatformIO + Wokwi
```

---

## docs/

| Pasta / ficheiro | Conteúdo |
|----------------|----------|
| [README.md](README.md) | Índice da documentação |
| [CONTEXTO_PROJETO.md](CONTEXTO_PROJETO.md) | Contexto global |
| [ETAPAS/](ETAPAS/) | E00–E12 — [índice](ETAPAS/README.md) |
| [TASKS_PAP_SYNC.md](TASKS_PAP_SYNC.md) | Tasks sincronizadas da app |
| [DIFICULDADES.md](DIFICULDADES.md) | Registo de problemas |
| [APRESENTACAO_CURSOR.md](APRESENTACAO_CURSOR.md) | Demo apresentação |
| [compras/](compras/) | Faturas e justificações |
| [relatorios/](relatorios/) | PDF/DOCX formais |

---

## elevador/

| Pasta | Conteúdo | Etapa |
|-------|----------|-------|
| [README.md](../elevador/README.md) | Índice elevador | — |
| [simulacao/](../elevador/simulacao/) | Tinkercad v01–v02, Wokwi v03–v06 | E01–E06 |
| [real/](../elevador/real/) | Firmware L298N + Hall + SH1106 | E09 |
| [testes/](../elevador/testes/) | Bancada + virtual | E07 |
| [maquete/](../elevador/maquete/) | Montagem física, vídeos | E08 |
| [hardware/](../elevador/hardware/) | Esquemas, BOM, mecânica | E08 |
| [documentacao/](../elevador/documentacao/) | Pinagem, estados | — |

### Simulação — versões

| Versão | Pasta Wokwi |
|--------|-------------|
| v01 | [tinkercad/v01](../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/) |
| v02 | [tinkercad/v02](../elevador/simulacao/legacy/tinkercad/v02_fila_pedidos/) |
| v03 | [wokwi/v03](../elevador/simulacao/legacy/wokwi/v03_transicao_mega_a4988/wokwi/) |
| v04 | [wokwi/v04](../elevador/simulacao/legacy/wokwi/v04_7segmentos_reset_rearme/wokwi/) |
| v05 | [wokwi/v05](../elevador/simulacao/legacy/wokwi/v05_oled_tca9548a/wokwi/) |
| **v06** | [wokwi/v06](../elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/) |

### Hardware elevador

| Pasta | Conteúdo |
|-------|----------|
| [esquemas/](../elevador/hardware/esquemas/) | [ELV-SCH-001 sim](../elevador/hardware/esquemas/elevador_v06_forca_comando.svg) · [ELV-SCH-002 real](../elevador/hardware/esquemas/elevador_real_forca_comando.svg) |
| [bom/](../elevador/hardware/bom/) | Lista de componentes |
| [mecanica/](../elevador/hardware/mecanica/) | Madeira + recipiente (não 3D) |

### Testes bancada

| Teste | Pasta |
|-------|-------|
| Botões + LEDs | [pedidos_exteriores/](../elevador/testes/bancada/pedidos_exteriores/) |
| OLED | [oled/](../elevador/testes/bancada/oled/) |
| Hall | [sensores_hall/](../elevador/testes/bancada/sensores_hall/) |

### Firmware real

```bash
cd elevador/real/l298n_sh1106_hall && pio run
```

---

## logicas_extras/

| Módulo | Estado | Simulação |
|--------|--------|-----------|
| [cofre/](../logicas_extras/cofre/) | **Wokwi OK** | [wokwi/](../logicas_extras/cofre/simulacao/wokwi/) |
| [paineis_controlo/](../logicas_extras/paineis_controlo/) | **Wokwi OK** | [wokwi/](../logicas_extras/paineis_controlo/simulacao/wokwi/) |
| [porta_predio/](../logicas_extras/porta_predio/) | Inicial | Relacionado com [cofre](../logicas_extras/cofre/) |
| [meteorologia/](../logicas_extras/meteorologia/) | No painel ecra | [paineis_controlo](../logicas_extras/paineis_controlo/) |
| [seguranca/](../logicas_extras/seguranca/) | PIR no painel ecra | [paineis_controlo](../logicas_extras/paineis_controlo/) |
| [rfid/](../logicas_extras/rfid/) | Inicial | RC522 comprado |
| [toldo_automatico/](../logicas_extras/toldo_automatico/) | Planeado | — |
| [legacy/](../logicas_extras/legacy/) | Histórico | — |

Índice: [logicas_extras/README.md](../logicas_extras/README.md) · Etapa [E12](ETAPAS/relatorios/E12_logicas_extras.md)

### Compilar painel ecra (ESP32)

```bash
cd logicas_extras/paineis_controlo/real/caixa_ecra && pio run
```

---

## interface/

| Ficheiro | Conteúdo |
|----------|----------|
| [README.md](../interface/README.md) | geral-controll-app — tarefas e organização |

Workspace recomendado: `PAP` + pasta local `me` (Candeias Control App).

---

## Etapas ↔ pastas (resumo)

| ID | Nome | Pasta principal |
|----|------|-----------------|
| E00 | Planeamento | [docs/relatorios/](relatorios/) |
| E01–E02 | Tinkercad | [elevador/simulacao/legacy/tinkercad/](../elevador/simulacao/legacy/tinkercad/) |
| E03–E06 | Wokwi | [elevador/simulacao/legacy/wokwi/](../elevador/simulacao/legacy/wokwi/) |
| E07 | Bancada | [elevador/testes/bancada/](../elevador/testes/bancada/) |
| E08 | Maquete | [elevador/maquete/](../elevador/maquete/) |
| E09 | Firmware real | [elevador/real/](../elevador/real/) |
| E10 | Comissionamento | _(futuro)_ |
| E11 | Apresentação | [APRESENTACAO_CURSOR.md](APRESENTACAO_CURSOR.md) |
| E12 | Lógicas extras | [logicas_extras/](../logicas_extras/) |

---

## Comandos rápidos (Cursor)

| Tarefa | Comando / pasta |
|--------|-----------------|
| Simular elevador v06 | `elevador/.../v06_.../wokwi/` → Wokwi Start |
| Firmware Mega real | `pio run` em `elevador/real/l298n_sh1106_hall/` |
| Painel ESP32 | `pio run` em `logicas_extras/paineis_controlo/real/caixa_ecra/` |
| Cofre Wokwi | `logicas_extras/cofre/simulacao/wokwi/` |

---

## Repositórios externos

| Repo | URL |
|------|-----|
| **PAP (este)** | https://github.com/candeiassantiago393-spec/PAP.git |
| Legado (migrado) | https://github.com/candeiassantiago393-spec/Elevador-Projeto-Final-Curso-PAP- |
| App organização | https://github.com/candeiassantiago393-spec/geral-controll-app |
