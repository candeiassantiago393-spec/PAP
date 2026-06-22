# E08 — Montagem na Maquete Física

| Campo | Valor |
|-------|-------|
| **Estado** | **Em curso** |
| **Período** | Junho 2026 — |
| **Registo** | [elevador/maquete/ESTADO_ATUAL.md](../../elevador/maquete/ESTADO_ATUAL.md) |

## Objetivo

Integrar na estrutura física os componentes já validados em simulação e bancada.

## Pipeline

```text
Programar → Simulação → Bancada → Maquete
   ✓          ✓           ✓        EM CURSO
```

## O que foi feito

| Componente | Estado |
|------------|--------|
| Botões pedido exterior (×4) | Montado — **piso 3 com falha** |
| LEDs pedido exterior (×4) | Montado — piso 3 a corrigir |
| Displays OLED (×4) | Montado + testados |
| TCA9548A | Montado |
| Guias cabos (argolas + braçadeiras) | Montado |
| Estrutura madeira + cabine (recipiente) | Montado |
| Motor NEMA 17 + tambor/correia | Conjunto mecânico pronto; **bancada OK**; shaft pendente |
| Driver L298N | Testado na bancada (tutor Manuel Queiroz) |
| Sensores Hall A3144 (×4) | Por montar/testar |
| Botões/LEDs interiores | Por decidir (possível só virtual) |
| Permissiva porta | Por decidir |
| Buzzer + reset | Pendente |
| Ventoinha L298N | Não comprada |
| Tambor + cabo de aço no shaft | Pendente integração |

## Decisões técnicas

- Montar I/O visível primeiro (botões, LEDs, displays)
- Motor e Hall a seguir (maior impacto mecânico)
- **Estrutura mecânica:** madeira + recipiente como cabine
- **Rés-do-chão** = 1.º andar na nomenclatura da maquete
- **Lógicas extras** na apresentação física: foco no elevador; extras depois ou em Wokwi

## Dificuldades

### Maus contactos e cabos curtos

Na montagem física surgiram **falhas intermitentes por mau contacto** e **cabos demasiado curtos** para a estrutura da maquete.

**Solução:** jumpers, ponteiras e **argolas guia** para cablagem organizada.

### Botão/LED 3.º andar

Ver [DIFICULDADES.md](../../DIFICULDADES.md) — em investigação.

## Resultado / validação

- Vídeos 2026-06-22: OLEDs + botões/LEDs exteriores
- Foto: guias de cabos e motor/tambor

## Evidências

| Ficheiro | Conteúdo |
|----------|----------|
| [oled_displays_teste.mp4](../../elevador/maquete/imagens/videos/2026-06-22_maquete_oled_displays_teste.mp4) | Teste 4 OLED |
| [botoes_leds_exteriores.mp4](../../elevador/maquete/imagens/videos/2026-06-22_maquete_botoes_leds_exteriores.mp4) | Botões/LEDs (piso 3 falha) |
| [guias_cabos_argolas.png](../../elevador/maquete/imagens/fotos/2026-06-22_guias_cabos_argolas.png) | Organização cabos |
| [motor_nema17_tambor_correia.png](../../elevador/hardware/mecanica/imagens/2026-06-22_motor_nema17_tambor_correia.png) | Transmissão |

## Próximo passo

→ **Testar Hall A3144** (casa) → montar nos pisos → integrar motor no shaft
