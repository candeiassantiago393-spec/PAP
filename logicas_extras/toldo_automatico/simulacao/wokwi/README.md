# Estendal Automático — Simulação Wokwi

Estendal/toldo que recolhe automaticamente quando o sensor de chuva deteta precipitação.

## Funcionamento

1. O sensor de chuva (simulado por um **slide switch** no pino D2) indica o estado do tempo.
2. Quando deteta chuva (`HIGH`): o **servo** (D9) roda de 0° → 90° para recolher/proteger o estendal.
3. Mantém-se recolhido durante 8 s.
4. Volta a 90° → 0° (reabre) e fica à espera de nova deteção.

## Componentes (simulação)

| Componente | Pino | Função |
|------------|------|--------|
| Arduino Uno | — | Controlador |
| Sensor de chuva (slide switch) | D2 | Deteção de precipitação |
| Servo | D9 | Recolher/abrir estendal |

## Simular no Cursor

1. Abrir `sketch.ino` nesta pasta.
2. `F1` → **Wokwi: Start Simulator**.
3. Mudar o slide switch para simular chuva e ver o servo a atuar.

## Wokwi

| Campo | Valor |
|-------|-------|
| Link | https://wokwi.com/projects/468789716183085057 |
| Autor | Santiago Candeias |

## Navegação

| | |
|--|--|
| Módulo | [../../README.md](../../README.md) |
| Lógicas extras | [../../../README.md](../../../README.md) |
