# Circuito v06 — Wokwi com pedidos interiores, permissiva de porta e OLEDs

Esta versão corresponde à versão atual/consolidada da simulação do elevador de 4 pisos em Wokwi.

Nesta fase, o projeto já utiliza Arduino Mega, motor de passo com driver A4988, quatro displays OLED através de multiplexador TCA9548A, botões exteriores, botões interiores da cabine, LEDs exteriores, LEDs interiores, sensores de piso simulados, buzzer, botão de reset e permissiva de porta.

Esta versão é uma das mais completas do desenvolvimento, pois aproxima a simulação do funcionamento esperado de um elevador real.

---

## Objetivo desta versão

O objetivo principal desta versão foi completar a lógica de pedidos e segurança do elevador, adicionando:

- botões interiores da cabine;
- LEDs interiores da cabine;
- permissiva de porta;
- paragem temporizada em piso pedido;
- bloqueio de movimento com porta aberta;
- gestão conjunta dos pedidos interiores e exteriores;
- apresentação dos estados nos OLEDs.

---

## Componentes utilizados

- Arduino Mega;
- motor de passo;
- driver A4988;
- multiplexador I2C TCA9548A;
- 4 displays OLED SSD1306;
- 4 botões exteriores de chamada;
- 4 botões interiores da cabine;
- 4 LEDs exteriores de pedidos;
- 4 LEDs interiores de pedidos;
- 4 sensores de piso simulados por interruptores;
- 1 interruptor para permissiva de porta;
- 1 botão de reset;
- buzzer;
- resistências;
- breadboards e cablagem.

---

## Principais novidades desta versão

Em relação à versão anterior, esta fase acrescenta:

- pedidos interiores da cabine;
- LEDs interiores da cabine;
- permissiva de porta;
- debounce da porta;
- paragem controlada de 10 segundos em piso pedido;
- bloqueio do motor quando a porta está aberta;
- estado `PORTA` apresentado nos OLEDs;
- atendimento de pedidos feitos no próprio piso atual;
- organização dos botões exteriores e interiores numa fila comum de pedidos.

---

## Pedidos exteriores e interiores

Nesta versão, o sistema passa a ter dois conjuntos de botões:

### Botões exteriores

```cpp
#define pinBotaoExtA 2
#define pinBotaoExtB 3
#define pinBotaoExtC 4
#define pinBotaoExtD 5
```

---

## Wokwi

| Campo | Valor |
|-------|-------|
| **Link** | https://wokwi.com/projects/464344193596402689 |
| **Export local** | [wokwi/](wokwi/) |
| **Firmware simulação** | `wokwi/sketch.ino` |
| **Cópia no repo** | `elevador_v06_wokwi_mega_pedidos_internos_porta_oled.ino` (igual ao sketch) |
| **Bibliotecas** | hd44780, Adafruit GFX, Adafruit SSD1306 |
| **Versão de referência** | **Simulação atual para apresentação** |

### Novidades face à v05

- Botões e LEDs interiores da cabine
- Permissiva de porta (D22)
- Paragem temporizada de 10 s em piso pedido
- Estado `PORTA` nos OLEDs

### Simular no Cursor

1. Abrir pasta `wokwi/` como raiz do projeto Wokwi
2. Extensão **Wokwi Simulator** → `F1` → *Wokwi: Start Simulator*

---

## Etapa do projeto

[E06 — Wokwi v06](../../../../docs/ETAPAS/relatorios/E06_wokwi_v06.md) — **versão de referência**
