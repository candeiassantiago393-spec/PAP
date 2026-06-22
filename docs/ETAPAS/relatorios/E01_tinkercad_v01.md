# E01 — Tinkercad v01 — Prova de Conceito

| Campo | Valor |
|-------|-------|
| **Estado** | Concluída |
| **Período** | Outubro 2025 _(mês a seguir ao planeamento, set. 2025)_ |
| **Código e simulação** | [elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/) |

## Objetivo

Validar a lógica básica do elevador de 4 pisos no Tinkercad: botões, sensores simulados, motor DC, LCD, LEDs e buzzer.

## O que foi feito

- Primeiro circuito funcional com Arduino Uno
- Motor DC com ponte H (L293D) — controlo de sentido
- LCD I2C 16×2 para indicar piso (nota: ideal 20×4 no projeto final)
- 4 botões exteriores + 4 LEDs + 4 interruptores como sensores de piso
- Buzzer para tom de chegada ao andar
- Fonte externa + regulador 5 V para Arduino e ponte H
- Breadboard esquerda: simulação de movimento / sensores fim de curso
- Anotações no circuito sobre evolução futura (Hall por andar, display 20×4)

## Evolução do código

Funcionou **inicialmente**, mas foi necessário **bastante tempo de otimização**: a primeira versão do programa usava lógicas muito básicas e teve de evoluir iterativamente até ficar utilizável.

## Componentes

Ver [README da v01](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/README.md) e [lista de componentes](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/imagens/lista_componentes.png).

## Limitações identificadas

- Sem fila de pedidos (só último destino) → levou à v02
- Arduino Uno com poucos pinos → levou à v03 (Mega + Wokwi)
- Motor DC sem posição precisa → motor de passo mais tarde

## Dificuldades

- Otimização da lógica de controlo (várias iterações até comportamento aceitável)

## Resultado / validação

- Elevador virtual responde a pedidos e para no piso certo
- Base sólida para v02 (fila) e transição Wokwi

## Evidências

| Ficheiro | Descrição |
|----------|-----------|
| [tinkercad_anotado.png](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/imagens/tinkercad_anotado.png) | Circuito com comentários |
| [tinkercad_sem_anotacoes.png](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/imagens/tinkercad_sem_anotacoes.png) | Circuito limpo |
| [circuito.png](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/imagens/circuito.png) | Imagem do circuito (repo) |
| [lista_componentes.png](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/imagens/lista_componentes.png) | BOM visual |
| [Elevador_PaP.pdf](../../../elevador/simulacao/legacy/tinkercad/v01_uno_motor_dc_lcd/documentacao/Elevador_PaP.pdf) | Documentação da fase |

## Próximo passo

→ [E02 — Fila de pedidos](E02_tinkercad_v02.md)
