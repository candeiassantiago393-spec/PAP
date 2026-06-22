# Justificação de Componentes — PAP

Cada componente adquirido está ligado à **função no sistema**, à **etapa** do projeto e à **fatura** Mauser.

## Índice por subsistema

| Documento | Conteúdo |
|-----------|----------|
| [elevador.md](elevador.md) | Fonte, OLEDs, resistências, cablagem — E07/E08 |
| [logicas_extras.md](logicas_extras.md) | ESP32, RFID, teclado, câmara — E12 |

## Tabela geral (todos os componentes documentados)

| Cód. Mauser | Componente | Função no projeto | Etapa | Fatura |
|-------------|------------|-------------------|-------|--------|
| 035-0873 | Fonte Orno 12 V 5 A 60 W | Alimenta motor (L298N) e malha de potência da maquete | E07 | [2026EC1454381](../faturas_mauser/2026EC1454381.md) |
| 095-0403 | Kit 525 resistências | Limitar corrente em LEDs, divisores e circuitos auxiliares | E07/E08 | [2026EC1454381](../faturas_mauser/2026EC1454381.md) |
| 096-8737 | OLED 1,3" I2C (×4) | Mostrar piso, fila, estado e emergência em cada andar | E07/E08 | [2026EC1454381](../faturas_mauser/2026EC1454381.md) |
| 016-0210 | Fio cobre preto 25 m | Cablagem fiável na maquete — sinais e alimentação | E08 | [recibo Corroios](../faturas_mauser/2026-06-15_recibo_corroios.md) |
| 016-0211 | Fio cobre branco 25 m | Par de cablagem identificada (retorno/GND/sinais) | E08 | [recibo Corroios](../faturas_mauser/2026-06-15_recibo_corroios.md) |
| 096-1690 | Manga termorretrátil | Isolar e fixar terminações — evitar mau contacto | E08 | [recibo Corroios](../faturas_mauser/2026-06-15_recibo_corroios.md) |
| 095-4900 | NodeMCU ESP32 | Microcontrolador Wi-Fi/BT para subsistemas do prédio | E12 | [2026EC1403323](../faturas_mauser/2026EC1403323.md) |
| 096-4642 | Teclado matricial 3×4 | Introduzir código de acesso à porta do prédio | E12 | [2026EC1403323](../faturas_mauser/2026EC1403323.md) |
| 096-7806 | OLED 0,96" | Ecrã de teste/protótipo para módulos extras | E12 | [2026EC1403323](../faturas_mauser/2026EC1403323.md) |
| 096-7946 | Placa photoshield | Prototipagem rápida de circuitos extras em bancada | E12 | [2026EC1403323](../faturas_mauser/2026EC1403323.md) |
| 096-8517 | RFID RC522 | Ler cartões de acesso (porta prédio / apartamentos) | E12 | [2026EC1403323](../faturas_mauser/2026EC1403323.md) |
| 095-5977 | T-Camera S3 | Câmara + PIR para vigilância/segurança do prédio | E12 | [2026EC1409725](../faturas_mauser/2026EC1409725.md) |
| 095-5979 | Caixa T-Camera | Proteger módulo de câmara na montagem | E12 | [2026EC1409725](../faturas_mauser/2026EC1409725.md) |
| 095-5980 | Cabo PH2.0 → DuPont | Ligar periféricos da T-Camera ao ESP32 | E12 | [2026EC1409725](../faturas_mauser/2026EC1409725.md) |

## Componentes pendentes (faturas por adicionar)

| Componente | Função prevista | Etapa |
|------------|-----------------|-------|
| Motor passo NEMA 17 | Movimentar cabine via cabo e tambor | E07/E08 |
| Driver L298N | Comandar motor bipolar na maquete real | E07 |
| Sensores Hall (×4) | Detetar chegada a cada piso | E08 |
| Multiplexador TCA9548A | Ligar 4 OLEDs no mesmo barramento I2C | E07 |
| Jumpers / rolos | Ligações rápidas em breadboard e maquete | E08 |
| Ponteiras (ferrules) | Terminações fiáveis em bornes e parafusos | E08 |
| Ventoinha 40 mm | Refrigeração do dissipador L298N | E08 |
| Botões, LEDs, buzzer | I/O do elevador (pedidos, alarme) | E07/E08 |

## Formato de cada entrada

```markdown
### Nome do componente (cód. Mauser)
- **Função:** o que faz no sistema
- **Onde:** elevador / lógica extra / bancada
- **Etapa:** E07, E08, E12…
- **Fatura:** link
```

Novos componentes seguem este modelo em [elevador.md](elevador.md) ou [logicas_extras.md](logicas_extras.md).
