# Simulação — Código Porta Prédio

Projecto Wokwi em [wokwi/](wokwi/).

## Compilar (obrigatório antes de simular)

A extensão Wokwi **não compila** `.ino` — é preciso gerar o firmware:

```bash
cd logicas_extras/cofre/simulacao/wokwi
python -m platformio run
```

Depois: **Stop + Start** na simulação.

## Teste rápido

1. Clicar nas teclas `1` `9` `0` `4` `#`
2. Servo abre, espera 8 s, fecha
3. Código errado → LED vermelho

## Ficheiros

| Ficheiro | Função |
|----------|--------|
| `sketch.ino` | Firmware |
| `diagram.json` | Circuito |
| `wokwi.toml` | Aponta para `.pio/build/uno/firmware.hex` |
| `platformio.ini` | Board uno + Keypad + Servo |

## Navegação

| | |
|--|--|
| Módulo | [../../README.md](../../README.md) |
| Wokwi detalhe | [wokwi/README.md](wokwi/README.md) |
