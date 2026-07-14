# Código Prédio — Teclado + Servo

Subsistema de **acesso por código numérico** à porta do prédio na maquete: teclado matricial 3×4 (096-4642) e servo.

| Campo | Valor |
|-------|-------|
| **Estado** | Simulação Wokwi funcional |
| **MCU** | Arduino Uno |
| **Código** | `1904` + `#` para confirmar |
| **Etapa** | [E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md) |

---

## Funcionamento

1. Introduzir dígitos no teclado 3×4
2. `#` confirma o código
3. **Correcto** (`1904`): servo abre suavemente (0°→90°), **fica 8 s aberto**, fecha suavemente (90°→0°)
4. **Incorrecto**: servo não move
5. `*` limpa a entrada

Detalhes: [documentacao/funcionamento.md](documentacao/funcionamento.md)

---

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [simulacao/wokwi/](simulacao/wokwi/) | **Simular aqui** — Wokwi + PlatformIO |
| [documentacao/](documentacao/) | Pinagem e fluxo |
| [real/](real/) | Firmware físico (`pio run -t upload`) |
| [testes/](testes/) | Testes bancada — **pendente** |
| [tranca_keypad_arduino/](tranca_keypad_arduino/) | Repo original + vídeo demo |

---

## Upload na bancada

```bash
cd logicas_extras/codigo_predio/real
pio run -t upload --upload-port COMx
```

## Simular no Cursor

```bash
cd logicas_extras/codigo_predio/simulacao/wokwi
python -m platformio run
```

1. Abrir `sketch.ino` → `F1` → **Wokwi: Start Simulator**
2. Introduzir `1904` + `#`

| Campo | Valor |
|-------|-------|
| Wokwi online | https://wokwi.com/projects/462220624733136897 |

---

## Hardware

| Componente | Simulação | Real (Mauser) |
|------------|-----------|---------------|
| Arduino Uno | ✓ | ✓ |
| Teclado 3×4 | Membrane keypad Wokwi | [096-4642](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| Servo | ✓ | Micro servo |

Pinagem: [documentacao/pinagem.md](documentacao/pinagem.md)

---

## Relacionado

| Módulo | Ligação |
|--------|---------|
| Cartões de prédio | [../cartoes_predio/](../cartoes_predio/) — acesso alternativo RFID |
| Central de controlo | [../central_de_controlo/](../central_de_controlo/) — alarmes |

## Navegação

| | |
|--|--|
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
