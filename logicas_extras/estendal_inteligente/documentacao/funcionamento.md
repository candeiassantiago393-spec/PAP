# Funcionamento — Estendal Inteligente

## Lógica

```text
loop:
  ler sensor chuva (digital)
  se HIGH (chuva):
    servo 0 → 90° (passo a passo, delay 50 ms)
    esperar 8000 ms
    servo 90 → 0° (passo a passo, delay 50 ms)
```

## Simulação vs real

| | Wokwi | Real (Temu) |
|---|-------|-------------|
| Sensor | Slide switch D2 | Módulo sensor chuva (saída digital) |
| Lógica HIGH = chuva | Igual quando módulo real entrega HIGH com água | |

## Comportamento do servo

| Ângulo | Significado na maquete |
|--------|------------------------|
| 0° | Estendal aberto / estendido |
| 90° | Estendal recolhido / protegido da chuva |

## Melhorias futuras

- Debounce no sensor (evitar falsos positivos)
- Não reabrir enquanto sensor indicar chuva
- Integrar com central de controlo (alerta “chuva” no OLED)
- ESP32 no topo do prédio em vez de Uno dedicado
