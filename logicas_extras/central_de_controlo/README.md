# Central de Controlo — ESP32 ou Mega (candeias.dev)

Projeto interativo com OLED, botões e vários subsistemas: ambiente, segurança, estufa, gás, fogo, sismo e portfolio. Compila para **ESP32** (caixa final / Wokwi) ou **Arduino Mega 2560** (bancada, padrão elevador).

**Autor / marca:** [candeias.dev](https://candeias.dev) · Instagram [@candeias.dev](https://www.instagram.com/candeias.dev)

---

## Funcionalidades

| Sistema | Descrição |
|--------|-----------|
| **HOME** | Marca `candeias.dev` / `studio` |
| **AMBIENTE** | Temperatura e humidade do ar (KY-015 / DHT11) |
| **MOVIMENTO** | PIR, cooldown 30 s, ecrã de alerta de intruso |
| **PLANTAS** | Humidade do solo, barra 0–100 %, LEDs, bomba com limite de tempo |
| **GRAFICO** | Sparkline das últimas leituras de temperatura |
| **GAS** | Nível de gás (0–100 %), barra com limite; alarme (buzzer + LED amarelo a piscar + ecrã "FUGA DE GAS!!") acima de 50 % |
| **FOGO** | Intensidade de chama (0–100 %), barra com limite; alarme (buzzer + LED vermelho a piscar + ecrã "FOGO / INCENDIO") acima de 50 % |
| **SISMO** | Intensidade sísmica (0–100 %), barra com limite; alarme (buzzer + LED verde a piscar + ecrã "SISMO / TERRAMOTO") acima de 50 % |
| **Instagram** | QR code para o perfil (último ecrã) |

**Prioridade dos alertas:** FOGO > SISMO > INTRUSO (PIR) > GÁS.

**Extra:** transições laterais entre ecrãs, indicador de página (pontinhos), **modo feira** (rotação automática a cada 5 s; pausa ao carregar botões; volta após 2 min sem interação).

Lógica da estufa baseada no [Projeto-Estufa v1.3](https://github.com/candeiassantiago393-spec/Projeto-Estufa) (solo &lt; 30 % SECO, 30–70 % IDEAL, &gt; 70 % MOLHADO).

---

## Hardware

Pinagem completa em [`documentacao/pinagem.md`](documentacao/pinagem.md) e [`pins.h`](pins.h).

### ESP32 (caixa final)

| Função | GPIO |
|--------|------|
| OLED SDA / SCL | 21 / 22 |
| Botão NEXT / OK | 32 / 33 |
| PIR | 27 |
| Buzzer | 25 |
| KY-015 (DHT11) | 26 |
| Solo (potenciómetro / analógico) | 34 |
| Gás (MQ-2/MQ-135 real; potenciómetro na simulação) | 35 |
| Fogo/chama (KY-026 real, saída analógica; potenciómetro na simulação) | 4 |
| Sismo/vibração (SW-420 real, digital; potenciómetro na simulação) | 13 |
| LED vermelho / amarelo / verde | 16 / 17 / 18 |
| Bomba (LED na simulação) | 19 |

### Arduino Mega 2560 (bancada)

| Função | Pin |
|--------|-----|
| OLED SDA / SCL | 20 / 21 |
| KY-015 (DHT11) | 2 |
| Botão NEXT / OK | 3 / 4 |
| PIR / Buzzer | 5 / 6 |
| LED vermelho / amarelo / verde | 7 / 8 / 9 |
| Bomba | 10 |
| Solo / Gás / Fogo / Sismo | A0 / A1 / A2 / A3 |

---

## Compilar e upload

```powershell
cd logicas_extras/central_de_controlo

# ESP32 (caixa / Wokwi)
pio run -e esp32dev -t upload --upload-port COMx

# Arduino Mega 2560 (bancada)
pio run -e megaatmega2560 -t upload --upload-port COMx
```

---

## Estrutura do projeto

```
central_de_controlo/
├── pins.h              # Pinagem ESP32 vs Mega
├── sketch.ino          # Sketch Arduino (Wokwi ESP32)
├── src/main.cpp        # Cópia PlatformIO (manter em sync)
├── diagram.json        # Circuito Wokwi
├── wokwi.toml          # Firmware compilado
├── platformio.ini
├── documentacao/       # funcionamento.md, pinagem.md
├── real/               # Montagem física (pendente)
├── testes/             # Registo testes (pendente)
└── README.md
```

---

## Compilar e simular

### Requisitos

- [PlatformIO](https://platformio.org/) (extensão VS Code/Cursor ou CLI)
- Extensão Wokwi (opcional)

### Build

```bash
python -m platformio run
```

### Wokwi

1. Compila o projeto (`platformio run`).
2. Abre a pasta no Wokwi / extensão Wokwi.
3. Após alterar código: **Stop + Start** na simulação (usa `.pio/build/esp32dev/firmware.bin`).

### Sincronizar sketch e PlatformIO

`sketch.ino` e `src/main.cpp` devem estar iguais. Ao editar um, copia para o outro ou mantém ambos atualizados.

---

## Navegação

- **NEXT** — ecrã seguinte (6 ecrãs, ciclo).
- **OK** — silencia alarme de intruso (durante alerta PIR).

---

## Próximas melhorias

Ideias para evolução do projeto (hardware, software e integração). Itens já rejeitados ou feitos estão marcados.

### Já implementado

- [x] Barra de humidade do solo
- [x] Indicador de ecrã (pontinhos)
- [x] Cooldown PIR (30 s)
- [x] Bomba com tempo máximo e pausa
- [x] QR Instagram (último ecrã)
- [x] Modo feira (auto-rotação + retoma após 2 min idle)
- [x] Gráfico de temperatura (sparkline)

### Ambiente e clima

| ID | Ideia | Hardware extra |
|----|--------|----------------|
| A1 | Qualidade do ar (CO₂ / VOC) | MH-Z19, SGP30 ou CCS811 |
| A2 | Pressão atmosférica / “vai chover?” | BMP280 ou BME280 |
| A3 | Índice de conforto (heat index) | Só software (KY-015) |
| A4 | Alertas de limiar de temperatura | Só software |
| A5 | Estação mini (chuva, vento, luz) | LDR, sensor chuva, anemómetro |

### Segurança e presença

| ID | Ideia | Hardware extra |
|----|--------|----------------|
| S1 | Porta / janela (contacto magnético) | Reed switch + íman |
| S2 | Deteção de vidro / vibração | Sensor piezo |
| S3 | Fumo / gás | MQ-2 / MQ-135 |
| S4 | Luz + movimento (noite) | LDR + PIR |
| S5 | Snapshot em intruso | ESP32-CAM |
| S6 | Sirene com tom alternado (alto/baixo) | Só software |

### Plantas e água

| ID | Ideia | Hardware extra |
|----|--------|----------------|
| P1 | Sensor de solo capacitivo (real) | Chirp / capacitivo I2C |
| P2 | Nível do reservatório (“água baixa”) | HC-SR04 ou float |
| P3 | pH / condutividade (hidroponia) | Sonda pH |
| P4 | Horas de luz para plantas | LDR ou BH1750 |
| P5 | Rega só de manhã / por horário | RTC DS3231 ou NTP |
| P6 | Multi-zona (2 solos, 2 bombas) | 2× sensores |
| P7 | Temp interior vs exterior (estufa) | 2× BME/DHT |

### Energia e caixa

| ID | Ideia | Hardware extra |
|----|--------|----------------|
| E1 | Bateria portátil | TP4056 + LiPo |
| E2 | Monitorizar corrente da bomba | ACS712 |
| E3 | Deep sleep + acordar com PIR | Software + MOSFET |

### Conforto e automação

| ID | Ideia | Hardware extra |
|----|--------|----------------|
| C1 | Presença mmWave (menos falsos que PIR) | LD2410 |
| C2 | Nível de ruído | MAX4466 |
| C3 | Relé (lâmpada, ventoinha) | Módulo relé |
| C4 | Perfis / cenas (Noite, Estufa, Segurança) | Só software |

### Conectividade (ESP32 Wi‑Fi)

| ID | Ideia |
|----|--------|
| W1 | Dashboard web na rede local |
| W2 | Alertas Telegram / Discord |
| W3 | Home Assistant / MQTT |
| W4 | Log na nuvem (InfluxDB / Grafana) |
| W5 | OTA (atualizar sem USB) |

### UX e portfolio (sem sensores novos)

| ID | Ideia |
|----|--------|
| U1 | Ícones por ecrã (termómetro, folha, escudo…) |
| U2 | Animação “studio” a piscar no HOME |
| U3 | Contador de deteções PIR no ecrã MOVIMENTO |
| U4 | Gráfico de humidade do ar (ecrã dedicado) — **OK bancada 12 jul** |
| U5 | Easter egg (combinação de botões) |
| U6 | Guardar limites 35 % / 75 % na EEPROM |

### Prioridade sugerida

1. **P1 + P2** — solo real + nível de água (caixa física)
2. **S1** — reed na tampa/porta
3. **A2** — BME280 (ambiente completo)
4. **W2** — Telegram
5. **T5** — sensor de gás MQ na bancada (próximo passo, ver [REGISTO_2026-07-12.md](testes/bancada/REGISTO_2026-07-12.md))
6. **U4** — gráfico humidade ar — concluído na bancada

### Não planeado (decisão anterior)

- Modo “em casa” (desativar PIR)
- LED azul de estado PIR
- Ecrã RESUMO único
- Rega manual com OK
- Shutdown com OK longo

---

## Licença

Projeto pessoal / portfolio. Usa e adapta livremente; referência a [candeias.dev](https://candeias.dev) apreciada.

---

## Referências

- [Projeto-Estufa](https://github.com/candeiassantiago393-spec/Projeto-Estufa) — lógica estufa v1.3
- [Wokwi ESP32](https://docs.wokwi.com/guides/esp32)
- [PlatformIO ESP32](https://docs.platformio.org/en/latest/platforms/espressif32.html)
