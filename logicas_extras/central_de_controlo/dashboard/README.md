# Dashboard — Smart Home Lab

Interface web local com gráficos ao vivo (temperatura, humidade, gás, vibração).

**Secções:** estado do sistema (ecrã OLED, buzzer, COM, uptime) · ambiente · segurança · gráficos · pinagem.

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
{"t":24.5,"h":55.0,"gas":12,"gr":380,"gb":350,"alarm":0,"qk":0,"qr":0,"qa":0,"scr":0,"ms":123456}
```

| Campo | Significado |
|-------|-------------|
| t | Temperatura °C |
| h | Humidade % |
| gas | Gás % |
| gr / gb | Raw / baseline A1 |
| alarm | 1 = alarme gás (buzzer) |
| qk / qr / qa | Vibração % / DO A3 / alarme sismo |
| scr | Ecrã OLED (0–4) |
