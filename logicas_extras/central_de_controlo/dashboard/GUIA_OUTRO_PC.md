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

## 2. Ligar hardware

1. Liga a **Mega** por USB.
2. **Fecha** Serial Monitor / Arduino IDE (libertar a COM).

O script **deteta a porta COM sozinho** — nao precisas de a descobrir manualmente.

Se quiseres ver as portas à mão:

```powershell
pio device list
```

---

## 3. Comando principal (upload + dashboard)

Na pasta raiz do projeto (`PAP\`):

```powershell
.\run_dashboard.cmd
```

**É só isto.** Funciona em qualquer PC com tudo instalado.

O script:
1. **Procura** a Arduino Mega ligada por USB
2. Compila e faz upload do firmware
3. Instala dependências Python (se faltarem)
4. Abre o browser em **http://127.0.0.1:8765**
5. Mantém o servidor a correr (fecha com `Ctrl+C`)

### Variantes

| Situação | Comando |
|----------|---------|
| Firmware já carregado | `.\run_dashboard.cmd -SkipUpload` |
| Porta manual (raro) | `.\run_dashboard.cmd -Port COM9` |

---

## 4. Variantes avançadas

| Situação | Comando |
|----------|---------|
| Só compilar/upload | `cd logicas_extras\central_de_controlo\testes\bancada\dht_gas_oled_next_buzzer` → `pio run -t upload` |
| Só dashboard manual | `cd logicas_extras\central_de_controlo\dashboard` → `py -3 server.py` (porta via `CENTRAL_COM` ou auto) |

---

## 5. Primeira utilização (~1 min)

1. **Fecha** Serial Monitor / Arduino IDE (libertar a COM).
2. Corre `.\run_dashboard.cmd`.
3. Espera **~30 s** — aquecimento do sensor MQ (normal).
4. No browser: ponto verde = Mega ligada; gráficos enchem aos poucos.

---

## Problemas comuns

### `PSSecurityException` / "running scripts is disabled"

Usa o ficheiro **`.cmd`** na raiz — não precisa de alterar políticas:

```powershell
.\run_dashboard.cmd
```

Se correres o `.ps1` directamente, usa:

```powershell
powershell -ExecutionPolicy Bypass -File logicas_extras\central_de_controlo\scripts\run_dashboard.ps1
```

### Nenhuma Arduino Mega encontrada

- Liga a Mega por USB (cabo de dados, não só carregamento).
- Fecha Serial Monitor, Arduino IDE e outros programas que usem a COM.
- Desliga/religa o cabo USB.
- Verifica no Gestor de dispositivos: **Portas (COM e LPT) → Arduino Mega 2560**.

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

- Outro programa ocupa a COM → fecha tudo excepto o dashboard.
- Cabo USB mau contacto.
- Mega desligada ou porta errada → corre de novo `.\run_dashboard.cmd`.

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
PAP/
├── run_dashboard.cmd            ← comando one-click (raiz do projeto)
logicas_extras/central_de_controlo/
├── scripts/run_dashboard.ps1    ← script principal
├── dashboard/
│   ├── server.py
│   ├── static/                  ← UI web
│   └── GUIA_OUTRO_PC.md         ← este guia
└── testes/bancada/dht_gas_oled_next_buzzer/   ← firmware
```

---

## Apresentação PAP (dica)

1. Liga Mega + sensores **antes** de abrir o dashboard.
2. Corre `.\run_dashboard.cmd` com o portátil na projeção.
3. Browser em ecrã completo (F11).
4. Aproxima gás ao MQ para demonstrar alarme + buzzer + gráfico a subir.

---

## Links

- Repo: https://github.com/candeiassantiago393-spec/PAP.git
- Dashboard local: http://127.0.0.1:8765
