# Teste — Sensores Hall

Validação dos 4 sensores de piso **A3144** (Hall digital TO-92) antes da integração na maquete.

| Campo | Valor |
|-------|-------|
| Modelo | **A3144** |
| Lógica | LOW = íman detetado |
| Íman na cabine | **Ainda não montado** |

| Ficheiro | Descrição |
|----------|-----------|
| [src/main.ino](src/main.ino) | Firmware com saída CSV 0–100 % para gráfico |
| [plot_hall.py](plot_hall.py) | Gráfico em tempo real (4 cores) |
| [sensorhall.ino](sensorhall.ino) | Teste simples só Serial Monitor (legado) |

## Lógica

- `LOW` = íman detetado (sensor activo)
- Firmware suaviza leituras digitais → **0–100 %** (zona limite oscila naturalmente)
- **≥ 55 %** no gráfico = magnetizado (linha mais grossa + área preenchida)

## Pinagem (maquete)

| Sensor | Pino |
|--------|------|
| Piso 1 | D8 |
| Piso 2 | D11 |
| Piso 3 | D12 |
| Piso 4 | D13 |

## Gráfico Python (recomendado)

### 1. Dependências (uma vez)

```bash
cd elevador/testes/bancada/sensores_hall
pip install -r requirements.txt
```

### 2. Carregar firmware no Mega

```bash
pio run -t upload
```

> Fecha o Serial Monitor do Cursor/PlatformIO antes de correr o Python (só uma app pode usar a COM).

### 3. Abrir gráfico

```bash
python plot_hall.py
```

Ou com porta explícita:

```bash
python plot_hall.py COM5
python plot_hall.py --list
```

### Cores no gráfico

| Piso | Cor |
|------|-----|
| 1 (D8) | Vermelho |
| 2 (D11) | Azul escuro |
| 3 (D12) | Verde |
| 4 (D13) | Amarelo |

## Teste simples (sem gráfico)

Abrir `sensorhall.ino` no Arduino IDE ou copiar lógica — mensagens de texto no Serial a 9600 baud.

## Etapa

[E07 — Testes de bancada](../../../../docs/ETAPAS/relatorios/E07_testes_bancada.md) → [E08 — Maquete](../../../../docs/ETAPAS/relatorios/E08_montagem_maquete.md)

## Navegação

| | |
|--|--|
| Bancada | [../README.md](../README.md) |
| Pinagem completa | [../../../documentacao/pinagem.md](../../../documentacao/pinagem.md) |
| Firmware real | [../../../real/l298n_sh1106_hall/](../../../real/l298n_sh1106_hall/) |
