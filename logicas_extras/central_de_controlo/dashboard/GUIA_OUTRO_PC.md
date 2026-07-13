# Guia — Dashboard Central de Controlo noutro PC

Como usar o dashboard web (gráficos temp/hum/gás) num computador diferente — escola, casa do colega, portátil de apresentação PAP.

---

## O que vais obter

- **Upload** do firmware na Arduino Mega (PlatformIO)
- **Dashboard** local em http://127.0.0.1:8765
- Gráficos e alarmes em tempo real via **Serial**

---

## Requisitos (instalar uma vez por PC)

| Software | Para quê | Onde |
|----------|----------|------|
| **Git** | Trazer o projeto | [git-scm.com](https://git-scm.com/) |
| **Cursor** ou VS Code | Editar e terminal | [cursor.com](https://cursor.com/) |
| **PlatformIO IDE** (extensão) | `pio run`, upload Mega | Extensões do Cursor |
| **Python 3.10+** | Servidor dashboard | [python.org](https://www.python.org/) — marcar **Add to PATH** |

Hardware: **Arduino Mega 2560** + cabo USB + sensores (KY-015, MQ, OLED na bancada).

---

## 1. Obter o projeto

### Opção A — Git clone (recomendado)

```powershell
git clone https://github.com/candeiassantiago393-spec/PAP.git
cd PAP
```

### Opção B — Cópia manual

Copia a pasta `PAP` (USB, OneDrive, etc.) para o outro PC.

---

## 2. Ligar hardware e descobrir a porta COM

1. Liga a **Mega** por USB.
2. No terminal do Cursor:

```powershell
[System.IO.Ports.SerialPort]::GetPortNames()
```

Anota a porta — ex.: `COM3`, `COM7`. **Muda consoante o PC.**

Também podes ver em **Gestor de dispositivos → Portas (COM e LPT) → Arduino Mega 2560**.

---

## 3. Comando principal (upload + dashboard)

```powershell
cd logicas_extras\central_de_controlo\scripts
.\run_dashboard.ps1 -Port COMx
```

Substitui **COMx** pela tua porta (ex.: `-Port COM7`).

O script:
1. Compila e faz upload do firmware
2. Instala dependências Python (se faltarem)
3. Abre o browser em **http://127.0.0.1:8765**
4. Mantém o servidor a correr (fecha com `Ctrl+C`)

---

## 4. Variantes úteis

| Situação | Comando |
|----------|---------|
| Firmware já carregado | `.\run_dashboard.ps1 -SkipUpload -Port COMx` |
| Só compilar/upload | `cd ..\testes\bancada\dht_gas_oled_next_buzzer` → `pio run -t upload --upload-port COMx` |
| Só dashboard manual | `cd ..\dashboard` → `$env:CENTRAL_COM='COMx'` → `py -3 server.py` |

---

## 5. Primeira utilização (~1 min)

1. **Fecha** Serial Monitor / Arduino IDE (libertar a COM).
2. Corre `.\run_dashboard.ps1 -Port COMx`.
3. Espera **~30 s** — aquecimento do sensor MQ (normal).
4. No browser: ponto verde = Mega ligada; gráficos enchem aos poucos.

---

## Problemas comuns

### `Acesso negado` / upload falha na COM

- Fecha **Serial Monitor**, **pio device monitor** e Arduino IDE.
- Desliga/religa USB da Mega.
- Tenta outra vez.

### `python` não encontrado

O script usa **`py -3`** automaticamente. Testa:

```powershell
py -3 --version
py -3 -m pip install -r logicas_extras\central_de_controlo\dashboard\requirements.txt
```

### Dashboard abre mas "Mega desligada"

- Porta COM errada → usa `-Port COMx` correcto.
- Outro programa ocupa a COM → fecha tudo excepto o dashboard.
- Cabo USB mau contacto.

### `pio` não reconhecido

- Instala extensão **PlatformIO IDE** no Cursor.
- Reinicia o Cursor.
- Abre a pasta `PAP` como workspace.

### Gráficos vazios nos primeiros segundos

- Normal durante aquecimento MQ (~30 s).
- KY-015 demora ~2 s após ligar.

---

## Pinagem Mega (referência)

| Componente | Pin |
|------------|-----|
| KY-015 (S) | 2 |
| BTN NEXT | 3 |
| Buzzer | 6 |
| MQ AOUT | A1 |
| OLED SDA / SCL | 20 / 21 |

Firmware: `testes/bancada/dht_gas_oled_next_buzzer/`

---

## Protocolo Serial (9600 baud)

```json
{"t":24.5,"h":55.0,"gas":12,"gr":380,"gb":350,"alarm":0,"scr":0,"ms":123456}
```

---

## Estrutura de ficheiros

```text
logicas_extras/central_de_controlo/
├── scripts/run_dashboard.ps1    ← comando one-click
├── dashboard/
│   ├── server.py
│   ├── static/                  ← UI web
│   └── GUIA_OUTRO_PC.md         ← este guia
└── testes/bancada/dht_gas_oled_next_buzzer/   ← firmware
```

---

## Apresentação PAP (dica)

1. Liga Mega + sensores **antes** de abrir o dashboard.
2. Corre `.\run_dashboard.ps1 -Port COMx` com o portátil na projeção.
3. Browser em ecrã completo (F11).
4. Aproxima gás ao MQ para demonstrar alarme + buzzer + gráfico a subir.

---

## Links

- Repo: https://github.com/candeiassantiago393-spec/PAP.git
- Dashboard local: http://127.0.0.1:8765
