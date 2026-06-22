# Justificação — Elevador (E07 / E08)

Componentes do **subsistema elevador**: alimentação, interface visual, cablagem e auxiliares.

---

## Fonte de alimentação industrial 12 VDC 5 A 60 W — Orno (035-0873)

| | |
|---|---|
| **Função** | Alimentar a **malha de potência** da maquete: motor de passo via L298N, com margem para picos de arranque. A lógica (Arduino Mega) pode ser alimentada a partir desta fonte ou em paralelo conforme o esquema final. |
| **Porquê esta especificação** | 12 V compatível com L298N e motor NEMA 17; 5 A / 60 W dimensionados no [Dimensionamentos_motor.docx](../../relatorios/Dimensionamentos_motor.docx). |
| **Etapa** | E07 — testes de bancada com carga real |
| **Fatura** | [2026EC1454381](../faturas_mauser/2026EC1454381.md) — 7,72 € |
| **Nota** | Atraso na entrega (abril 2026) bloqueou temporariamente os testes integrados. |

---

## Módulo ecrã OLED 1,3" I2C branco (096-8737) — ×4 unidades

| | |
|---|---|
| **Função** | **Interface homem-máquina** do elevador: em cada piso (ou zona), mostrar número do piso actual, sentido de movimento, fila de pedidos, estado do sistema (`NORMAL`, `PORTA`, `ERRO`, `REARME`). Substitui LCD/7 segmentos da simulação. |
| **Porquê 4 unidades** | Um display por piso do prédio em escala, alinhado com a simulação Wokwi v05/v06. |
| **Controlador** | SH1106 na realidade (ajuste de biblioteca face ao SSD1306 do Wokwi). |
| **Etapa** | E07 (bancada) → E08 (maquete) |
| **Fatura** | [2026EC1454381](../faturas_mauser/2026EC1454381.md) — 22,24 € (4× 5,56 €) |

---

## Conjunto 525 resistências 1/4 W 1% (095-0403)

| | |
|---|---|
| **Função** | **Limitar corrente** nos LEDs de pedido (exteriores e interiores), divisores de tensão e outros circuitos auxiliares na breadboard e na maquete. |
| **Porquê kit** | Vários valores num só conjunto — útil em prototipagem sem encomendar valor a valor. |
| **Etapa** | E07 / E08 |
| **Fatura** | [2026EC1454381](../faturas_mauser/2026EC1454381.md) — 5,24 € |

---

## Bobine fio cobre multifilar preto 25 m — 0,14 mm² (016-0210)

| | |
|---|---|
| **Função** | **Cablagem permanente** na maquete com comprimento adequado; um condutor identificado (preto) para sinais ou alimentação conforme convenção do projeto. |
| **Porquê** | Resolver **mau contacto** e **cabos demasiado curtos** dos jumpers iniciais. |
| **Etapa** | E08 — montagem física |
| **Fatura** | [Recibo Corroios 2026-06-15](../faturas_mauser/2026-06-15_recibo_corroios.md) — 3,55 € |

---

## Bobine fio cobre multifilar branco 25 m — 0,14 mm² (016-0211)

| | |
|---|---|
| **Função** | Par de cablagem com o fio preto — identificação de condutores (retorno, GND ou segundo polo) em ligações mais longas na estrutura da maquete. |
| **Etapa** | E08 |
| **Fatura** | [Recibo Corroios 2026-06-15](../faturas_mauser/2026-06-15_recibo_corroios.md) — 2,56 € |

---

## Manga termorretrátil amarela 1 m (096-1690)

| | |
|---|---|
| **Função** | **Isolar e reforçar** junções de fio após crimpagem ou torção — reduz falsos contactos e curtos-circuitos acidentais na maquete. |
| **Etapa** | E08 |
| **Fatura** | [Recibo Corroios 2026-06-15](../faturas_mauser/2026-06-15_recibo_corroios.md) — 0,30 € |

---

## Componentes elevador — faturas pendentes

| Componente | Função prevista |
|------------|-----------------|
| Motor passo NEMA 17 | Converter rotação em movimento vertical da cabine (cabo + tambor) |
| Driver L298N | Acionar bobinas do motor bipolar com corrente da fonte 12 V |
| Sensores Hall (×4) | Detetar presença da cabine em cada piso (substituem interruptores da simulação) |
| TCA9548A | Multiplexar 4 OLEDs com o mesmo endereço I2C no Mega |
| Jumpers / rolos | Ligações de desenvolvimento e testes |
| Ponteiras | Terminações em parafusos/bornes para ligação fiável |
| Ventoinha 40 mm | Arrefecer dissipador do L298N durante operação prolongada |
| Botões exteriores/interiores | Pedidos de chamada à cabine e em cada piso |
| LEDs de pedido | Indicar visualmente pedidos pendentes |
| Buzzer | Bip de chegada e alarme contínuo em emergência |
