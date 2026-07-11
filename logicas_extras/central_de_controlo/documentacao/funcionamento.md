# Funcionamento — Central de Controlo

Caixa ESP32 com OLED 128×64, botões NEXT/OK e vários subsistemas. Marca: **candeias.dev**.

## Ecrãs (9 páginas)

| # | Ecrã | Conteúdo |
|---|------|----------|
| 0 | HOME | Logo candeias.dev / studio |
| 1 | AMBIENTE | Temperatura e humidade (KY-015 / DHT11) |
| 2 | MOVIMENTO | Estado PIR, cooldown 30 s |
| 3 | PLANTAS | Humidade solo, barra, bomba, LEDs |
| 4 | GAS | Nível gás, barra, limite 50 % |
| 5 | FOGO | Intensidade chama, limite 50 % |
| 6 | SISMO | Intensidade vibração, limite 50 % |
| 7 | GRAFICO | Sparkline temperatura |
| 8 | Instagram | QR code @candeias.dev |

**NEXT** — ecrã seguinte (ciclo). **OK** — silencia alarme de intruso (PIR).

## Modo feira

Rotação automática entre ecrãs a cada 5 s. Pausa ao carregar botões; retoma após 2 min sem interacção.

## Estufa (PLANTAS)

Baseado no [Projeto-Estufa v1.3](https://github.com/candeiassantiago393-spec/Projeto-Estufa):

| Humidade solo | Estado | Acção |
|---------------|--------|-------|
| &lt; 30 % | SECO | LED vermelho, bomba ON (máx. 10 s), verde pisca, buzzer 2 kHz |
| 30–70 % | IDEAL | LED verde fixo, bomba OFF |
| &gt; 70 % | MOLHADO | LED amarelo, buzzer 500 Hz |

Bomba: pausa de 30 s entre ciclos se solo continuar seco.

## Alarmes e prioridade

Quando um limiar é ultrapassado, o ecrã normal é substituído por um **alerta fullscreen**:

| Alarme | Condição | LED | Mensagem ecrã |
|--------|----------|-----|---------------|
| **FOGO** | ≥ 50 % | Vermelho pisca | FOGO / INCENDIO |
| **SISMO** | ≥ 50 % | Verde pisca | SISMO / TERRAMOTO |
| **INTRUSO** | PIR HIGH | Vermelho pisca | ALERTA Intruso |
| **GÁS** | ≥ 50 % | Amarelo pisca | FUGA DE GAS!! |

**Prioridade:** FOGO &gt; SISMO &gt; INTRUSO &gt; GÁS. Alarmes de maior prioridade suspendem estufa e outros outputs.

## Simulação vs real

| Sensor | Wokwi (simulação) | Hardware real (kit Mauser) |
|--------|-------------------|----------------------------|
| Solo | Potenciómetro GPIO 34 | Sensor capacitivo / analógico |
| Gás | Potenciómetro GPIO 35 | MQ-2 / MQ-135 (saída analógica) |
| Fogo | Potenciómetro GPIO 4 | KY-026 (saída analógica AO) |
| Sismo | Potenciómetro GPIO 13 | SW-420 vibração (digital → converter pulsos) |
| Ambiente | KY-015 (DHT11) — Wokwi usa `wokwi-dht22` com firmware DHT11 | GPIO 26 |
| Movimento | PIR nativo Wokwi | PIR GPIO 27 |

## Compilar e simular

```bash
cd logicas_extras/central_de_controlo
python -m platformio run
```

Wokwi: Stop + Start após compilar (usa `.pio/build/esp32dev/firmware.bin`).

## Ficheiros de código

| Ficheiro | Função |
|----------|--------|
| `sketch.ino` | Sketch Arduino (Wokwi) |
| `src/main.cpp` | Cópia PlatformIO — **manter em sync** |
| `diagram.json` | Circuito Wokwi |
| `wokwi.toml` | Aponta para firmware compilado |
