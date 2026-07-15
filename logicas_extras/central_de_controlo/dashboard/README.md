# Dashboard — Smart Home Lab

Interface web local com gráficos ao vivo (temperatura, humidade, gás, **fogo**, vibração).

**Secções:** estado do sistema (ecrã OLED, buzzer, COM, uptime, fogo) · ambiente · segurança · gráficos · pinagem.

**Usar noutro PC:** [GUIA_OUTRO_PC.md](GUIA_OUTRO_PC.md)

## One-click (recomendado)

```powershell
cd logicas_extras\central_de_controlo\scripts
.\run_dashboard.ps1
```

Opções:
```powershell
.\run_dashboard.ps1 -Port COM7
.\run_dashboard.ps1 -SkipUpload   # só abrir dashboard (firmware já carregado)
```

## Manual

```powershell
# 1. Upload firmware (JSON na Serial)
cd logicas_extras\central_de_controlo\testes\bancada\dht_gas_oled_next_buzzer
pio run -t upload --upload-port COM7

# 2. Dashboard (fecha Serial Monitor antes!)
cd ..\..\..\dashboard
pip install -r requirements.txt
$env:CENTRAL_COM = "COM7"
python server.py
```

Abrir: http://127.0.0.1:8765

## Protocolo Serial (9600 baud)

Uma linha JSON por leitura:

```json
{"t":24.5,"h":55.0,"gas":12,"gr":380,"gb":350,"alarm":0,"fire":8,"fr":512,"fbl":540,"fd":28,"fdo":0,"fa":0,"qk":0,"qr":0,"qa":0,"scr":4,"fw":"3","ms":123456}
```

| Campo | Significado |
|-------|-------------|
| t | Temperatura °C |
| h | Humidade % |
| gas | Gás % |
| gr / gb | Raw / baseline A1 |
| alarm | 1 = alarme gás |
| fire | Fogo/chama % (KY-026 AO + DO) |
| fr / fbl / fd | Raw AO A2 / baseline / queda |
| fdo | 1 = saída digital DO deteta chama |
| fa | 1 = alarme fogo (prioridade máxima buzzer) |
| qk / qr / qa | Vibração % / sinal A3 / alarme sismo |
| scr | Ecrã OLED (0–5: HOME…FOGO…SISMO) |
| fw | Versão firmware telemetria (`3` = com fogo) |

Prioridade buzzer no firmware: **fogo > sismo > gás**.
