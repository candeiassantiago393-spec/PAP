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
| Botões pedido exterior (×4) | Montado |
| LEDs pedido exterior (×4) | Montado |
| Displays OLED (×4) | Montado |
| TCA9548A | Montado |
| Sensores Hall (×4) | Pendente |
| Motor NEMA 17 + L298N | Pendente |
| Botões/LEDs interiores | Pendente |
| Permissiva porta | Pendente |
| Buzzer + reset | Pendente |
| Estrutura madeira + cabine (recipiente) | Montado — fotos pendentes |
| Tambor + cabo de aço | Pendente |

## Decisões técnicas

- Montar I/O visível primeiro (botões, LEDs, displays)
- Motor e sensores a seguir (maior impacto mecânico)
- **Estrutura mecânica:** construção manual em **madeira** + **recipiente adaptado** como cabine (em vez de impressão 3D prevista no plano inicial)

## Dificuldades

### Maus contactos e cabos curtos

Na montagem física surgiram **falhas intermitentes por mau contacto** e **cabos demasiado curtos** para a estrutura da maquete.

**Solução:** aquisição de **rolos de jumpers** e **ponteiras (ferrules)** para ligações mais fiáveis e cablagem com comprimento adequado.

> Compras: [fios e termorretrátil](../../compras/faturas_mauser/2026-06-15_recibo_corroios.md) — recibo Mauser Corroios, 7,88 €.

### Registo contínuo

- Documentar cada problema de contacto resolvido à medida que a montagem avança
- Fotos antes/depois quando relevante

## Resultado / validação

- Vídeo 2026-06-22: primeira integração I/O exterior + OLEDs

## Evidências

- [Vídeo maquete](../../elevador/maquete/imagens/videos/2026-06-22_maquete_botoes_leds_displays.mp4)
- `elevador/hardware/mecanica/imagens/` _(estrutura madeira + cabine — a preencher)_

## Próximo passo

→ **Sensores Hall** (4 unidades, um por piso), depois motor + L298N
