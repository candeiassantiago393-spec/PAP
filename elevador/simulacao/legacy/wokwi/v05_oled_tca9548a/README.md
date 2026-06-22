# Circuito v05 — Wokwi com Arduino Mega, A4988 e 4 OLEDs via TCA9548A

Esta versão corresponde a uma das fases mais importantes do desenvolvimento da PAP.  
Nesta etapa, o projeto deixa de utilizar LCD e abandona a ideia experimental dos displays de 7 segmentos, passando a utilizar quatro displays OLED ligados por comunicação I2C através de um multiplexador TCA9548A.

Esta alteração melhora bastante a interface visual do sistema, permitindo apresentar mais informação sobre o estado do elevador, o piso atual, o sentido de movimento, o pedido mais recente, a fila de pedidos e o estado geral do sistema.

---

## Objetivo desta versão

O objetivo principal desta versão foi melhorar a interface visual do elevador, substituindo soluções anteriores mais limitadas por uma solução baseada em displays OLED.

Nesta fase, pretendeu-se:

- substituir o LCD I2C por displays OLED;
- abandonar a ideia dos displays de 7 segmentos;
- utilizar um multiplexador I2C TCA9548A;
- permitir a ligação de quatro displays OLED com o mesmo endereço I2C;
- apresentar informação mais detalhada ao utilizador;
- manter a lógica de pedidos;
- manter o controlo do motor de passo com A4988;
- manter a lógica de reset, erro e rearme;
- aproximar a simulação da versão final do projeto.

---

## Componentes utilizados

- Arduino Mega;
- motor de passo;
- driver A4988;
- multiplexador I2C TCA9548A;
- 4 displays OLED SSD1306;
- 4 botões exteriores de chamada;
- 4 LEDs indicadores de pedidos;
- 4 sensores de piso simulados por interruptores;
- buzzer;
- botão de reset;
- breadboards e cablagem.

---

## Principal novidade: quatro displays OLED

A principal novidade desta versão é a introdução de quatro displays OLED.

Cada display apresenta informação relevante sobre o funcionamento do elevador, como:

- número do OLED;
- piso atual;
- sentido do movimento;
- pedido mais recente;
- fila de pedidos;
- estado do sistema.

No código foram criadas quatro instâncias de display:

```cpp
Adafruit_SSD1306 oled1(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled2(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled3(OLED_W, OLED_H, &Wire, -1);
Adafruit_SSD1306 oled4(OLED_W, OLED_H, &Wire, -1);
```

---

## Wokwi

| Campo | Valor |
|-------|-------|
| **Link** | https://wokwi.com/projects/455660851315238913 |
| **Export local** | [wokwi/](wokwi/) |
| **Firmware simulação** | `wokwi/sketch.ino` |
| **Cópia no repo** | `elevador_v05_wokwi_mega_a4988_oled_tca9548a.ino` (igual ao sketch) |
| **Bibliotecas** | hd44780, Adafruit GFX, Adafruit SSD1306 |

### Novidades face à v04

- Abandono dos displays de 7 segmentos
- 4× OLED SSD1306 via multiplexador TCA9548A
- Interface visual rica (fila, sentido, estados)

### Simular no Cursor

1. Abrir pasta `wokwi/` como raiz do projeto Wokwi
2. Extensão **Wokwi Simulator** → `F1` → *Wokwi: Start Simulator*

---

## Etapa do projeto

[E05 — Wokwi v05](../../../../docs/ETAPAS/relatorios/E05_wokwi_v05.md)
