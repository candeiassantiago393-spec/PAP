# Real — Meteorologia

Na maquete, a meteorologia aparece no **ecra OLED por andar**. O protótipo actual é a caixa ESP32 do projeto ecra.

## Firmware (inclui meteorologia + outros ecrãs)

| Pasta | Descrição |
|-------|-----------|
| [paineis_controlo/real/caixa_ecra/](../../paineis_controlo/real/caixa_ecra/) | PlatformIO ESP32 — DHT22, ecrãs AMBIENTE e GRÁFICO |

```bash
cd logicas_extras/paineis_controlo/real/caixa_ecra
pio run -t upload
```

Funções meteorologia no código: `drawEnvScreen`, `drawGraphScreen`, `updateEnvironmentData`, `pushTempSample`.

## Montagem na maquete

| Fase | Estado |
|------|--------|
| Caixa demo única (feira / apresentação) | Em curso |
| Ecra fixo no hall de cada andar | Planeado |

Hardware por unidade: ESP32 + OLED 128×64 + DHT22 (ver [pinagem](../documentacao/pinagem.md)).

## Navegação

| | |
|--|--|
| Módulo | [../README.md](../README.md) |
| Painel completo | [../../paineis_controlo/real/](../../paineis_controlo/real/) |
