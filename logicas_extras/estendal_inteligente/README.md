# Estendal Inteligente — Prédio Inteligente

Estendal/toldo que **recolhe automaticamente** quando o sensor de chuva deteta precipitação.

## Objectivo na maquete

Sensor de chuva no **topo do prédio** → aciona o estendal na zona exterior da maquete.

| Campo | Valor |
|-------|-------|
| **Estado** | Simulação Wokwi funcional |
| **MCU** | Arduino Uno |
| **Actuador** | Servo (recolhe/abre) |
| **Etapa** | [E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md) |

---

## Funcionamento (resumo)

1. Sensor de chuva detecta precipitação
2. Servo roda 0° → 90° (recolhe estendal)
3. Mantém 8 s recolhido
4. Reabre 90° → 0°

Detalhes: [documentacao/funcionamento.md](documentacao/funcionamento.md)

---

## Estado hardware (jul 2026)

| Item | Estado |
|------|--------|
| Sensor chuva (Temu) | Comprado — por instalar no topo |
| Servo | Simulado — montar na maquete |
| Firmware real | **Pendente** |

---

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/wokwi/](simulacao/wokwi/) | **Simular aqui** |
| [documentacao/](documentacao/) | Funcionamento e pinagem |
| [real/](real/) | Montagem física — **pendente** |
| [testes/](testes/) | Teste molhado/seco — **pendente** |

---

## Simular no Cursor

```bash
cd logicas_extras/estendal_inteligente/simulacao/wokwi
python -m platformio run
```

Abrir `sketch.ino` → Wokwi Start → mudar **slide switch** (simula chuva).

| Wokwi online | https://wokwi.com/projects/468789716183085057 |

---

## Relacionado

| Módulo | Ligação |
|--------|---------|
| Central de controlo | [../central_de_controlo/](../central_de_controlo/) — ambiente interior (DHT22) |

## Navegação

| | |
|--|--|
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
