# Testes — Meteorologia

Checklist para validar DHT22 e ecrãs **AMBIENTE** / **GRÁFICO** no painel ecra.

## Pré-requisitos

Firmware carregado ou Wokwi a correr — ver [simulacao/](../simulacao/).

## Checklist

- [ ] Ecrã **AMBIENTE** mostra temperatura plausível (ex. 20–30 °C em sala)
- [ ] Ecrã **AMBIENTE** mostra humidade (ex. 40–70 %)
- [ ] Após ~2 s deixa de aparecer *"A ler DHT22..."*
- [ ] Ecrã **GRÁFICO** passa de *"A recolher..."* a curva após várias leituras
- [ ] Curva reage se alterares temperatura do DHT (soprar, aquecer com dedo)
- [ ] Botão **NEXT** alterna correctamente entre ecrãs

## Wokwi

1. [paineis_controlo/simulacao/wokwi/](../../paineis_controlo/simulacao/wokwi/)
2. Simular → NEXT até AMBIENTE / GRÁFICO

## Bancada física

1. `pio run -t upload` em [caixa_ecra](../../paineis_controlo/real/caixa_ecra/)
2. Alimentar ESP32 por USB
3. Repetir checklist

## Testes do painel (global)

[../../paineis_controlo/testes/README.md](../../paineis_controlo/testes/README.md)

## Navegação

| | |
|--|--|
| Módulo | [../README.md](../README.md) |
| Funcionamento | [../documentacao/funcionamento.md](../documentacao/funcionamento.md) |
