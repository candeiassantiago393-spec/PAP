# Simulação — Elevador

Versões virtuais do firmware, da prova de conceito (Tinkercad) ao sistema completo (Wokwi v06).

## Estrutura

```text
simulacao/
├── legacy/
│   ├── tinkercad/     → v01, v02 (migradas)
│   └── wokwi/         → v03–v06 (Arduino Mega, A4988, OLED)
│       └── <versão>/
│           ├── README.md
│           ├── elevador_v0X_....ino
│           ├── imagem_circuito.png
│           └── wokwi/           ← abrir esta pasta para simular no Cursor
│               ├── diagram.json
│               ├── sketch.ino
│               ├── wokwi.toml
│               └── ...
└── wokwi/
    └── exports/       → reserva para cópias adicionais
```

## Links Wokwi (v03–v06)

| Versão | Link | Pasta local |
|--------|------|-------------|
| v03 | [454346932045509633](https://wokwi.com/projects/454346932045509633) | `legacy/wokwi/v03_transicao_mega_a4988/wokwi/` |
| v04 | [454408518771138561](https://wokwi.com/projects/454408518771138561) | `legacy/wokwi/v04_7segmentos_reset_rearme/wokwi/` |
| v05 | [455660851315238913](https://wokwi.com/projects/455660851315238913) | `legacy/wokwi/v05_oled_tca9548a/wokwi/` |
| v06 | [464344193596402689](https://wokwi.com/projects/464344193596402689) | `legacy/wokwi/v06_atual_pedidos_internos_porta/wokwi/` |

**Versão de apresentação:** v06

## Simular no Cursor

1. Instalar extensão **Wokwi Simulator**
2. Abrir pasta `wokwi/` da versão desejada
3. `F1` → *Wokwi: Start Simulator*

## Migração

- Código legado: `Elevador-Projeto-Final-Curso-PAP-/circuitos/`
- Estado Wokwi v03–v06: **integrado** no repo `PAP`
