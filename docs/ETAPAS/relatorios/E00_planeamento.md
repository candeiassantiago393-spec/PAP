# E00 — Planeamento e Especificação

| Campo | Valor |
|-------|-------|
| **Estado** | Concluída |
| **Período** | Setembro 2025 — |

## Origem da ideia

O projeto de **prédio inteligente** surgiu por ser um tema que consegue reunir **alta diversidade de conteúdos** ao longo do curso de Técnico de Eletrónica, Automação e Comando — área muito ampla (sensores, motores, automação, interfaces, segurança, etc.).

## Definição inicial

- **Elevador de 4 pisos** como lógica principal
- **Prédio com lógicas extras** ainda por explorar na altura (segurança, RFID, meteorologia, porta, toldo, etc.)
- Abordagem: simulação virtual primeiro → maquete física depois

## Reuniões com orientadores

Nada de especial a registar para além do acompanhamento normal (Ricardo Feio, Manuel Queiroz, Rui Barros).

## O que foi feito

- Definição do elevador como subsistema principal
- Identificação das lógicas extras do prédio
- Relatório técnico e especificação (PDF v3.0)
- Escolha Arduino Mega
- Dimensionamento do motor (ver evidências)
- Pinagem, estados do sistema e requisitos de segurança

## Decisões técnicas

- 4 pisos fixos; porta manual com permissiva lógica
- Simulação (Tinkercad → Wokwi) antes da implementação real
- Motor de passo com transmissão cabo + tambor + cabine 3D
- L298N na maquete real (A4988 na simulação)

## Dificuldades

- Nenhuma relevante na fase de planeamento

## Resultado / validação

- Âmbito do projeto definido e aceite como base do desenvolvimento
- Documentação técnica iniciada

## Evidências

- [Dimensionamentos_motor.docx](../../relatorios/Dimensionamentos_motor.docx) — contas do motor (massa, roda, inércia, corrente)
- `Relatorio_Especificacao_Elevador.pdf` — [docs/relatorios/](../../relatorios/Relatorio_Especificacao_Elevador.pdf)
- `Relatorio_Elevador_Inteligente.docx` — [docs/relatorios/](../../relatorios/Relatorio_Elevador_Inteligente.docx)
- [CONTEXTO_PROJETO.md](../../CONTEXTO_PROJETO.md)

## Próximo passo

→ [E01 — Tinkercad v01](E01_tinkercad_v01.md)
