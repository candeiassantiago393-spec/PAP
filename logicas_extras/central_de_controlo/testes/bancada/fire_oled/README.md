# Teste — KY-026 (fogo) + OLED SSD1306 (Arduino Mega)

Validação isolada do **sensor de chama KY-026** e do display — bancada com **Arduino Mega 2560**.

## Ecrãs (só botão NEXT)

| Ecrã | Conteúdo |
|------|----------|
| HOME | Branding `candeias.dev studio` |
| FOGO | Barra 0–100 %, limite 50 %, estado OK/FOGO! |
| GRAF FOGO | Sparkline do histórico de intensidade |

**NEXT** (pin 3) avança HOME → FOGO → GRAF FOGO → HOME.

Se intensidade **≥ 50 %** → ecrã de alarme **FOGO / INCENDIO** + LED vermelho (pin 7) a piscar (prioridade sobre navegação).

## Hardware

| Componente | Ligação |
|------------|---------|
| Arduino Mega 2560 | USB |
| OLED SSD1306 128×64 | SDA **20**, SCL **21**, 0x3C, 5V, GND |
| KY-026 | **VCC** 5V, **GND** GND, **AO** → **A2**, **DO** → **pin 5** |
| Botão NEXT | Um terminal → **pin 3**, outro → **GND** (diagonal se 4 pinos) |
| LED vermelho (opcional) | **pin 7** + GND (com resistência ~220 Ω) |

### KY-026 — pinos típicos

```
(-) GND    (+) VCC    DO digital (nao usado neste teste)    AO -> A2
```

Neste teste usa-se só **AO** (analógico). Aproximar chama/isqueiro **a distância segura** aumenta a leitura.

## Upload

```powershell
cd logicas_extras/central_de_controlo/testes/bancada/fire_oled
pio run -t upload --upload-port COMx
pio device monitor -b 9600
```

## Serial (9600)

```
KY-026 A2 raw=512 -> 50% OK
BTN NEXT premido
Ecra: FOGO
KY-026 A2 raw=780 -> 76% ALARME
```

### KY-026 — 4 pinos (ligar AO **e** DO)

```
(-) GND    (+) VCC    DO -> pin 5    AO -> A2
```

**Importante:** muitos módulos só reagem bem pelo **DO** (digital). O LED do módulo deve acender perto da chama — se acende mas o Arduino não muda, falta ligar **DO ao pin 5**.

## KY-026 mostra ~5% e isqueiro não muda nada?

1. **Liga DO ao pin 5** — é o sinal principal em muitos KY-026
2. **Roda o potenciómetro** no módulo até o **LED do módulo** acender com o isqueiro
3. Ecrã **DIAG** (NEXT até lá): vê `DO pin5: LOW` com chama?
4. Se `AO` não muda mas `DO` muda → OK, detecção por DO
5. Se **nada muda** (AO e DO iguais): cabo errado, módulo avariado, ou isqueiro longe — aproxima a 1–3 cm

## KY-026 mostra ~90% sem chama? (versão antiga)

**Causa habitual:** saída analógica **AO invertida** — sem chama o pin A2 fica **alto** (~900 raw), com chama **desce**.

O firmware usa `FIRE_AO_INVERTED 1` (em `main.ino`) para corrigir: sem chama ≈ 10 %, com chama sobe.

| raw A2 (Serial) | Sem invert | Com invert |
|-----------------|------------|------------|
| ~920 | 90 % | **~10 %** |
| ~200 (com chama) | 20 % | **~80 %** |

Outras verificações:

1. **Potenciómetro no módulo** — roda para baixo a sensibilidade se existir
2. **Luz ambiente** — luz forte/IR pode falsear; tapa o sensor ou afasta de LEDs
3. **Fio certo** — usa **AO** → A2, não DO
4. Se com chama o % **desce** em vez de subir, mantém `FIRE_AO_INVERTED 1`
5. Se com chama o % **sobe** de um valor já alto, muda para `FIRE_AO_INVERTED 0`

## Critérios de sucesso

- [ ] OLED mostra HOME e responde a NEXT
- [ ] Sem chama: barra baixa, estado OK
- [ ] Com chama perto: percentagem sobe, gráfico desenha linha
- [ ] Acima de 50 %: alarme no OLED + LED vermelho pisca

## Referência firmware completo

Lógica alinhada com [../../../sketch.ino](../../../sketch.ino) — `readFirePercent()`, `FIRE_LIMIT`, ecrã FOGO.

## Registo

Teste **T6** — [../../README.md](../../README.md)
