# Estado Atual da Maquete

> Última atualização: 2026-06-22

## Fase do projeto

**Integração física na maquete** — componentes já validados em simulação (Wokwi v06) e em testes de bancada.

```text
[✓] Programar / projetar
[✓] Simulação virtual (v01–v06)
[✓] Testes de bancada (botões, LEDs, OLED, Hall)
[→] Montagem na maquete          ← ESTAMOS AQUI
[ ] Integração motor + movimento
[ ] Testes sistema completo
[ ] Acabamentos mecânicos (tambor, cabo) — estrutura madeira + cabine já feitas
```

---

## Componentes montados

| Componente | Qtd | Estado | Validado em |
|------------|-----|--------|-------------|
| Botões pedido exterior | 4 | Montado | Bancada + sim v06 |
| LEDs pedido exterior | 4 | Montado | Bancada |
| Displays OLED | 4 | Montado | Bancada (`oledbecnch`) |
| Multiplexador TCA9548A | 1 | Montado | Sim v05/v06 |

**Evidência:** [vídeo 2026-06-22](imagens/videos/2026-06-22_maquete_botoes_leds_displays.mp4)

---

## Próximos passos (ordem planeada)

1. **Sensores Hall** — 4 unidades, um por piso
2. **Motor de passo NEMA 17** + driver **L298N**
3. Botões interiores da cabine (4)
4. LEDs interiores (4)
5. Permissiva de porta (interruptor)
6. Buzzer + botão reset
7. Tambor + cabo de aço _(estrutura madeira e cabine/recipiente já montadas — fotos em `hardware/mecanica/imagens/`)_

---

## Diferenças simulação ↔ maquete

| Aspeto | Wokwi v06 | Maquete |
|--------|-----------|---------|
| Driver | A4988 | L298N |
| OLED | SSD1306 | SH1106 |
| Sensor piso | Interruptor | Hall |
| Motor | Stepper sim | NEMA 17 real |

---

## Notas de montagem

_(Adicionar aqui observações à medida que se monta — cablagem, fixações, problemas mecânicos.)_

- 2026-06-22: Primeira integração de I/O exterior e displays na estrutura física.
- Estrutura mecânica construída **à mão em madeira**; cabine = **recipiente adaptado** (não impressão 3D). Fotos a adicionar em [hardware/mecanica/imagens/](../hardware/mecanica/imagens/).
