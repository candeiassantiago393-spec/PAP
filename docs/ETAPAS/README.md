# Etapas do Projeto PAP

> Registo cronológico e técnico de cada fase do elevador e do prédio inteligente.  
> Cada etapa tem o seu relatório em `relatorios/` e liga ao código correspondente.

---

## Mapa geral

```text
FASE 0 ─ Planeamento e especificação
   │
FASE 1 ─ Simulação Tinkercad (v01–v02)
   │
FASE 2 ─ Transição Wokwi + Mega (v03)
   │
FASE 3 ─ Segurança e 7 segmentos (v04)
   │
FASE 4 ─ Interface OLED (v05)
   │
FASE 5 ─ Sistema completo simulado (v06)          ← referência simulação
   │
FASE 6 ─ Testes de bancada (hardware isolado)
   │
FASE 7 ─ Montagem na maquete física               ← EM CURSO
   │
FASE 8 ─ Firmware real (L298N + Hall + SH1106)
   │
FASE 9 ─ Integração e comissionamento
   │
FASE 10 ─ Documentação e relatório final
   │
FASE 11 ─ Apresentação (Cursor + 3D)
   │
FASE 12 ─ Lógicas extras do prédio (paralelo/futuro)
```

---

## Índice de etapas

| ID | Etapa | Estado | Relatório |
|----|-------|--------|-----------|
| E00 | [Planeamento e especificação](relatorios/E00_planeamento.md) | Concluída | [Dimensionamentos_motor.docx](../relatorios/Dimensionamentos_motor.docx) |
| E01 | [Tinkercad v01 — prova de conceito](relatorios/E01_tinkercad_v01.md) | Concluída | [v01_uno_motor_dc_lcd](../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/) |
| E02 | [Tinkercad v02 — fila de pedidos](relatorios/E02_tinkercad_v02.md) | Concluída | [v02_fila_pedidos](../../elevador/simulacao/legacy/tinkercad/v02_fila_pedidos/) |
| E03 | [Wokwi v03 — Mega + A4988](relatorios/E03_wokwi_v03.md) | Concluída | [v03/wokwi](../../elevador/simulacao/legacy/wokwi/v03_transicao_mega_a4988/wokwi/) |
| E04 | [Wokwi v04 — 7 seg + rearme](relatorios/E04_wokwi_v04.md) | Concluída | [v04/wokwi](../../elevador/simulacao/legacy/wokwi/v04_7segmentos_reset_rearme/wokwi/) |
| E05 | [Wokwi v05 — OLED TCA9548A](relatorios/E05_wokwi_v05.md) | Concluída | [v05/wokwi](../../elevador/simulacao/legacy/wokwi/v05_oled_tca9548a/wokwi/) |
| E06 | [Wokwi v06 — sistema completo](relatorios/E06_wokwi_v06.md) | Concluída | [v06/wokwi](../../elevador/simulacao/legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/) |
| E07 | [Testes de bancada](relatorios/E07_testes_bancada.md) | Concluída | [bancada](../../elevador/testes/bancada/) |
| E08 | [Montagem maquete](relatorios/E08_montagem_maquete.md) | Em curso | rascunho |
| E09 | [Firmware real L298N](relatorios/E09_firmware_real.md) | Iniciada | rascunho |
| E10 | [Comissionamento final](relatorios/E10_comissionamento.md) | Pendente | — |
| E11 | [Relatório e apresentação](relatorios/E11_apresentacao.md) | Pendente | — |
| E12 | [Lógicas extras prédio](relatorios/E12_logicas_extras.md) | Em curso | [cofre/wokwi](../../logicas_extras/cofre/simulacao/wokwi/) · [painel ecra](../../logicas_extras/paineis_controlo/simulacao/wokwi/) |

**Estados:** `Pendente` · `Em curso` · `Concluída` · `A documentar`

---

## Como funciona

1. Cada etapa tem um ficheiro `relatorios/EXX_*.md` com secções fixas
2. O assistente faz perguntas para preencher o que falta
3. Dificuldades relevantes são espelhadas em [DIFICULDADES.md](../DIFICULDADES.md)
4. Código e simulação ligam-se via links no relatório

---

## Template de cada relatório

```markdown
# EXX — Título
- Período:
- Estado:
- Objetivo:
- O que foi feito:
- Componentes / ferramentas:
- Decisões técnicas:
- Dificuldades:
- Resultado / validação:
- Evidências (fotos, links, vídeos):
- Próximo passo:
```

---

## Ligação ao código

| Etapas | Pasta no repo |
|--------|---------------|
| E01–E02 | `elevador/simulacao/legacy/tinkercad/` |
| E03–E06 | `elevador/simulacao/legacy/wokwi/` |
| E07 | `elevador/testes/bancada/` |
| E08 | `elevador/maquete/` |
| E09 | `elevador/real/` |
| E12 | `logicas_extras/` |

---

## Navegação

| | |
|--|--|
| Índice repo | [INDICE_REPOSITORIO.md](../INDICE_REPOSITORIO.md) |
| Documentação | [../README.md](../README.md) |
| Contexto | [../CONTEXTO_PROJETO.md](../CONTEXTO_PROJETO.md) |
| Raiz PAP | [../../README.md](../../README.md) |
