# Teste — KY-015 + OLED SSD1306 (Arduino Mega)

Validação isolada do sensor de humidade/temperatura e do display — na bancada com **Arduino Mega 2560** (mesmo padrão do elevador).

## O que valida

| Ecrã | Conteúdo |
|------|----------|
| HOME | `candeias.dev` + `studio` |
| AMBIENTE | Temperatura e humidade do KY-015 |
| GRAFICO TEMP | Histórico de temperatura (sparkline) |
| GRAFICO HUM | Histórico de humidade (sparkline) |
| INSTAGRAM | QR code + `@candeias.dev` |

Os ecrãs rodam automaticamente a cada 5 s **ou** com botões:
- **NEXT** (pin 3) — ecrã seguinte
- **PREV** (pin 4) — ecrã anterior

A consola Serial (9600) imprime as leituras a cada 2 s.

## Hardware

| Componente | Ligação |
|------------|---------|
| Arduino Mega 2560 | USB para upload + alimentação |
| OLED SSD1306 128×64 | SDA pin 20, SCL pin 21, endereço `0x3C`, VCC 5V, GND |
| KY-015 (DHT11) | **(-)** GND → GND Mega, **(S)** Data → pin 2 Mega, **(+)** VCC → 5V. Pull-up já integrado no módulo — **não** é preciso resistor externo |
| Botão NEXT | Um terminal → pin 3, **outro terminal (lado oposto)** → GND |
| Botão PREV | Um terminal → pin 4, **outro terminal (lado oposto)** → GND |

### Botão táctil 4 pinos (erro comum)

Vista de cima, pinos para baixo:

```
  [1]──[2]     ← lado A (ligados entre si)
  [3]──[4]     ← lado B (ligados entre si)
```

Premir o botão **liga A a B**. Usa pinos **em diagonal** (ex.: 1 → pin 3, 3 → GND).  
**Não** ligues os dois fios ao mesmo lado (1 e 2) — o botão não fecha o circuito.

No ecrã **HOME** aparece `N:1 P:1` em repouso; ao premir NEXT deve mudar para `N:0`.  
Na Serial: `BTN NEXT premido` e `Ecra: AMBIENTE`.

## KY-015 não lê? (`A ler KY-015...` no OLED)

1. Confirma pinagem do módulo (vista dos pinos, texto legível): **(-)=GND, (S)=Data, (+)=VCC**
2. **(S)** ligado ao **pin 2** da Mega (não confundir com SDA do OLED)
3. Alimentação **5V** no **(+)** — o KY-015 não funciona bem a 3,3 V
4. Abre Serial Monitor (9600) — deve aparecer `KY-015: XX.X C | XX.X %`
5. Espera **2–4 s** após ligar — primeira leitura pode falhar

**Nota:** O firmware completo em `../../` compila para Mega (`megaatmega2560`) e ESP32 (`esp32dev`) — mesma lógica, pinagem em [`../../pins.h`](../../pins.h).

## Upload

```powershell
cd logicas_extras/central_de_controlo/testes/bancada/dht_oled
pio run -t upload --upload-port COMx
pio device monitor
```

## Critérios de sucesso

- [ ] OLED liga e mostra HOME com branding
- [ ] KY-015 devolve valores plausíveis na Serial e no ecrã AMBIENTE
- [ ] Gráficos de temperatura e humidade desenham linha após ~4 s
- [ ] QR code visível no ecrã final
- [ ] LED pin 13 pisca (Mega activo)

## Registo

Registos diários: [REGISTO_2026-07-11.md](../REGISTO_2026-07-11.md) · [REGISTO_2026-07-12.md](../REGISTO_2026-07-12.md)

Preencher resultado em [../../README.md](../../README.md) — testes T1 (OLED) e T2 (KY-015).
