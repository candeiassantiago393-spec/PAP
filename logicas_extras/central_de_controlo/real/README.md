# Implementação real — Central de Controlo (Mega bancada)

Firmware **físico** validado na bancada — separado da simulação Wokwi/ESP32 na raiz do módulo.

## Workflow de versões

Cada **junção** (combinação de componentes) guarda snapshots em `versoes/<junção>/vN/`.

| Quando… | O que fazer |
|---------|-------------|
| Novo teste combinado funciona | Criar ou incrementar `versoes/<junção>/vN/` |
| Dizes **«este é o real mais recente»** | Copiar essa versão para [`atual/`](atual/) |
| Upload na bancada (versão activa) | `cd real/atual && pio run -t upload` |

## Junções registadas

| Junção | Componentes | Versões | Actual |
|--------|-------------|---------|--------|
| [dht_gas_oled_next_buzzer](versoes/dht_gas_oled_next_buzzer/) | KY-015 + MQ (AO) + OLED + NEXT + buzzer | **v1** | **sim** → [`atual/`](atual/) |

## Estrutura

```text
real/
├── README.md              ← este ficheiro (índice + workflow)
├── atual/                 ← cópia da versão aprovada mais recente
│   ├── platformio.ini
│   └── src/main.ino
└── versoes/
    └── <junção>/
        ├── README.md
        ├── v1/
        └── v2/ …
```

## Pinagem Mega (junção actual)

| Função | Pin |
|--------|-----|
| KY-015 (S) | 2 |
| BTN NEXT | 3 |
| Buzzer | 6 |
| MQ AOUT | A1 |
| OLED SDA / SCL | 20 / 21 |

Teste de desenvolvimento (mesmo código): [../testes/bancada/dht_gas_oled_next_buzzer/](../testes/bancada/dht_gas_oled_next_buzzer/)

## Por fazer (caixa ESP32 final)

Montagem ESP32 na maquete — ver plano original abaixo quando aplicável.

- [ ] ESP32 + OLED na caixa
- [ ] PIR, LEDs, solo, fogo, sismo…
- [ ] Pinagem definitiva em `pinagem.md`

Referência simulação: [../documentacao/pinagem.md](../documentacao/pinagem.md)
