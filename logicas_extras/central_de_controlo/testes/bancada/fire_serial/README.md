# Teste KY-026 — fogo (Serial, analógico + digital)

Detecção de chama com **os dois sinais** do módulo KY-026.

## Pinagem (Arduino Mega)

| KY-026 | Mega | Função |
|--------|------|--------|
| **(-) GND** | **GND** | Massa |
| **(+) VCC** | **5V** | Alimentação |
| **AO** | **A0** | Analógico — queda vs baseline = chama |
| **DO** | **pin 5** | Digital — **LOW** = chama (ajustar pot no módulo) |

```
    KY-026                    MEGA
    ------                    ----
    (-) GND  ---------------- GND
    (+) VCC  ---------------- 5V
    AO       ---------------- A0
    DO       ---------------- D5 (digital pin 5)
```

## Upload

```powershell
cd logicas_extras/central_de_controlo/testes/bancada/fire_serial
pio run -t upload --upload-port COMx
pio device monitor -b 9600
```

## Serial

```
A0=918  q=  0  DO=HIGH  ana=NAO  dig=NAO  ok
A0=720  q=198  DO=LOW   ana=SIM  dig=SIM  >>> FOGO <<<
```

| Campo | Significado |
|-------|-------------|
| `A0` | Leitura analógica (0–1023) |
| `q` | Queda vs baseline (sobe com chama) |
| `DO` | Estado do pin 5 (LOW = chama) |
| `ana=SIM` | Analógico detetou (queda ≥ 60) |
| `dig=SIM` | Digital detetou (DO LOW) |
| `>>> FOGO <<<` | Pelo menos um dos dois activo |

## Montagem

1. **Reinicia** o Mega **sem chama** perto (calibra baseline)
2. Roda o **potenciómetro** no KY-026 até o **LED do módulo** acender com isqueiro a 1–3 cm
3. Se só `dig=SIM` funciona → analógico fraco, OK
4. Se só `ana=SIM` funciona → DO mal ligado, verifica pin 5
5. Se nada muda → confirma AO→A0, DO→5, VCC 5V

## Limiar analógico

Em `main.ino`: `AO_ALERT_DROP 60` — baixa se for sensível demais, sobe se não disparar.
