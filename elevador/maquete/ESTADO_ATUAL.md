# Estado Atual da Maquete

> Última atualização: 2026-06-22 (registo utilizador + evidências)

## Fase do projeto

**Integração física na maquete** — I/O exterior e displays montados; motor testado em **bancada** (ainda não integrado na estrutura).

```text
[✓] Programar / projetar
[✓] Simulação virtual (v01–v06)
[✓] Testes de bancada (botões, LEDs, OLED, motor+L298N, Hall pendente teste casa)
[→] Montagem na maquete (I/O + cablagem)     ← ESTAMOS AQUI
[ ] Sensores Hall + íman na cabine
[ ] Motor + tambor na maquete
[ ] Testes sistema completo
[ ] Acabamentos (ventoinha L298N, porta/interiores se aplicável)
```

---

## Componentes montados na maquete

| Componente | Qtd | Estado | Notas |
|------------|-----|--------|-------|
| Botões pedido exterior | 4 | Montado | **Piso 3 com falha** — ver [DIFICULDADES](../../docs/DIFICULDADES.md) |
| LEDs pedido exterior | 4 | Montado | Piso 3 a investigar (mau contacto?) |
| Displays OLED SH1106 | 4 | Montado | Testados em vídeo |
| Multiplexador TCA9548A | 1 | Montado | — |
| Guias de cabos (argolas) | várias | Montado | Braçadeiras + argolas brancas na estrutura |
| Estrutura madeira + cabine | 1 | Montado | Recipiente adaptado |

### Ainda fora da maquete (mas disponíveis)

| Componente | Estado |
|------------|--------|
| Motor NEMA 17 + tambor/polia + correia | Montagem mecânica feita; **testado na bancada** com 12 V; por integrar no shaft |
| Driver L298N | Testado na bancada (cedido tutor Manuel Queiroz) |
| Sensores Hall **A3144** ×4 | Por soldar/testar — íman na cabine **ainda não** |
| Ventoinha 40 mm L298N | **Não comprada** |
| Botões/LEDs interiores | **Por decidir** — possível só apresentação virtual |
| Permissiva porta (D22) | **Por decidir** — possível só virtual |

---

## Próximos passos (ordem)

1. **Testar Hall A3144** em casa (`testes/bancada/sensores_hall/` + gráfico Python)
2. Soldar/montar Hall nos 4 pisos + fixar **ímã na cabine**
3. Integrar **motor + tambor** no topo/shaft da maquete
4. Corrigir **botão/LED 3.º andar** (testes de continuidade)
5. Ventoinha no L298N (quando comprada)
6. Decidir se porta e botões interiores ficam físicos ou só na simulação Wokwi

---

## Convenção de pisos

| Nome na maquete | Piso lógico firmware |
|-----------------|----------------------|
| **Rés-do-chão / 1.º andar** | Piso 1 (D8 Hall, botão D2, etc.) |
| 2.º–4.º andar | Pisos 2–4 |

---

## Evidências (fotos e vídeos)

| Ficheiro | Descrição |
|----------|-----------|
| [oled_displays_teste.mp4](imagens/videos/2026-06-22_maquete_oled_displays_teste.mp4) | 4 displays OLED a serem testados |
| [botoes_leds_exteriores.mp4](imagens/videos/2026-06-22_maquete_botoes_leds_exteriores.mp4) | Botões + LEDs exteriores — **piso 3 com problema** |
| [guias_cabos_argolas.png](imagens/fotos/2026-06-22_guias_cabos_argolas.png) | Argolas brancas + braçadeiras como guias de cabo |
| [motor_tambor_correia.png](../hardware/mecanica/imagens/2026-06-22_motor_nema17_tambor_correia.png) | NEMA 17, correia dentada, tambor como polia |

---

## Diferenças simulação ↔ maquete

| Aspeto | Wokwi v06 | Maquete |
|--------|-----------|---------|
| Driver | A4988 | L298N (D9, D33, D31, D32) |
| OLED | SSD1306 | SH1106 + TCA9548A |
| Sensor piso | Interruptor | Hall **A3144** (LOW = íman) |
| Motor | Stepper sim | NEMA 17 bipolar (4 fios) |
| Porta / interiores | Simulados | Possivelmente **só virtuais** na apresentação |

Pinagem completa: [documentacao/pinagem.md](../documentacao/pinagem.md)

---

## Notas de montagem

- 2026-06-22: I/O exterior + OLEDs integrados na estrutura madeira.
- Cablagem organizada com **braçadeiras** e **argolas** (tipo berço) parafusadas à maquete.
- Motor + tambor: conjunto mecânico pronto; alimentação 12 V validada **na bancada**, não no shaft.
- Lógicas extras (cofre, ecra, toldo, RFID): **após elevador funcional** na maquete.
