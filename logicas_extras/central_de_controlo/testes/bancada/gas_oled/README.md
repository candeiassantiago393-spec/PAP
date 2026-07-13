# Teste — MQ gás + OLED SSD1306 (Arduino Mega)

Validação isolada do **sensor de gás MQ-2 / MQ-135** e do display — bancada com **Arduino Mega 2560**.

## Ecrãs (botões NEXT e PREV)

| Ecrã | Conteúdo |
|------|----------|
| HOME | Branding `candeias.dev studio` |
| GAS | Barra 0–100 %, limite 50 %, estado OK/FUGA! |
| GRAF GAS | Sparkline do histórico de nível |
| DIAG | Raw AO, baseline, subida, estado DO |

**NEXT** (pin 3) avança · **PREV** (pin 4) recua — HOME → GAS → GRAF → DIAG → HOME.

Se nível **≥ 50 %** (ou DO activo) → ecrã **FUGA DE GAS!!** + LED amarelo (pin 8) a piscar.

## Hardware

| Componente | Ligação |
|------------|---------|
| Arduino Mega 2560 | USB |
| OLED SSD1306 128×64 | SDA **20**, SCL **21**, 0x3C, 5V, GND |
| MQ-2 / MQ-135 | **VCC** 5V, **GND** GND, **AOUT** → **A1**, **DO** → **pin 5** |
| Botão NEXT | Um terminal → **pin 3**, outro → **GND** |
| Botão PREV | Um terminal → **pin 4**, outro → **GND** |
| LED amarelo (opcional) | **pin 8** + GND (com resistência ~220 Ω) |

### MQ — pinos típicos

```
VCC    GND    DO -> pin 5    AO -> A1
```

O módulo precisa de **~30 s de aquecimento** após ligar (já incluído no `setup`).

## Upload

```powershell
cd logicas_extras/central_de_controlo/testes/bancada/gas_oled
pio run -t upload --upload-port COMx
pio device monitor -b 9600
```

## Serial (9600)

```
AO=412 base=380 rise=32 DO=- -> 12% OK
BTN NEXT premido
Ecra: GAS
BTN PREV premido
Ecra: HOME
AO=620 base=380 rise=240 DO=GAS -> 100% ALARME
```

## Ajustes

| Problema | Solução |
|----------|---------|
| Alarme cedo demais | Aumentar `GAS_LIMIT` ou `GAS_AO_RISE_FULL` em `main.ino` |
| Barra não reage | Ligar **DO** ao pin 5; rodar potenciómetro do módulo MQ |
| Sem DO ligado | Comentar `readGasDo()` e usar só AO |

## Critérios de sucesso

- [ ] OLED mostra HOME e responde a NEXT/PREV
- [ ] Ar limpo: barra baixa, estado OK
- [ ] Com gás perto: percentagem sobe, gráfico desenha linha
- [ ] Acima de 50 %: alarme no OLED + LED amarelo pisca

## Referência

Lógica alinhada com [../../../sketch.ino](../../../sketch.ino) — `readGasPercent()`, `GAS_LIMIT`, ecrã GAS.

Teste **T5** — [../README.md](../README.md)
