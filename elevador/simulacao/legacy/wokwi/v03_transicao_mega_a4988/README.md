# Circuito v03 — Transição para Wokwi com Arduino Mega e A4988

Esta versão representa uma fase importante do desenvolvimento da PAP, pois corresponde à transição do projeto do Tinkercad para o simulador Wokwi.

A mudança de plataforma ocorreu devido à necessidade de utilizar um Arduino Mega, uma vez que o projeto começou a exigir mais entradas e saídas do que aquelas que estavam disponíveis no Arduino Uno usado nas versões anteriores.

Além da mudança de simulador e microcontrolador, esta versão também introduz o uso de um motor de passo controlado por driver A4988, substituindo a solução anterior baseada em motor DC e ponte H.

## Motivo da transição para o Wokwi

Nas versões anteriores, o projeto foi desenvolvido no Tinkercad com Arduino Uno. Essa abordagem foi suficiente para testar a lógica inicial do elevador, incluindo botões exteriores, LEDs, sensores simulados, buzzer e LCD.

No entanto, à medida que o projeto evoluiu, tornou-se necessário adicionar mais elementos ao sistema, tais como:

- mais entradas digitais;
- mais saídas para LEDs;
- possibilidade de adicionar botões interiores;
- botão de reset;
- permissiva de porta;
- mais sensores;
- displays adicionais;
- controlo mais estruturado do motor;
- maior margem para expansão do código.

O Arduino Uno tornou-se limitado devido ao número reduzido de pinos disponíveis. Como o Tinkercad não disponibiliza o Arduino Mega da forma necessária para esta evolução, o projeto foi transferido para o Wokwi.

### Como se chegou ao Wokwi (novembro 2025)

Antes da adopção do Wokwi, ainda não estava definida a plataforma de simulação para o Arduino Mega. O orientador **Rui Barros**, por iniciativa própria, identificou o **Wokwi** como alternativa e comunicou essa opção ao autor via WhatsApp. A migração decorreu de forma estável e o Wokwi passou a ser a ferramenta de simulação utilizada nas versões seguintes.

## Objetivo desta versão

O objetivo desta versão foi adaptar o projeto para uma plataforma com maior capacidade de expansão, mantendo a lógica de pedidos já desenvolvida anteriormente.

Esta versão permitiu:

- migrar a simulação para Wokwi;
- substituir o Arduino Uno pelo Arduino Mega;
- substituir o motor DC por um motor de passo;
- utilizar o driver A4988 para comando do motor;
- manter a lógica de fila de pedidos;
- manter os sensores de piso simulados;
- manter os LEDs indicadores de pedidos;
- manter o buzzer;
- manter temporariamente o LCD I2C como interface visual.

## Principais alterações em relação à v02

- Transição de Tinkercad para Wokwi;
- substituição do Arduino Uno pelo Arduino Mega;
- substituição do motor DC por motor de passo;
- substituição da ponte H pelo driver A4988;
- comando do motor por sinais STEP e DIR;
- velocidades definidas em passos por segundo;
- introdução de controlo não bloqueante do motor;
- atualização do LCD limitada no tempo para não prejudicar o movimento do stepper;
- manutenção da lógica de pedidos através do array `pedidos[]`.

## Componentes utilizados nesta versão

- Arduino Mega;
- driver A4988;
- motor de passo;
- LCD 20x4 com comunicação I2C;
- 4 botões exteriores;
- 4 LEDs indicadores de pedidos;
- 4 resistências para LEDs;
- 4 interruptores deslizantes para simular sensores de piso;
- buzzer;
- breadboards e cablagem.

## Alteração no controlo do motor

Nas versões anteriores, o motor era um motor DC controlado por ponte H, usando sinais PWM.

Nesta versão, o motor passa a ser um motor de passo controlado por A4988. O controlo deixa de ser feito por PWM e passa a ser feito através de dois sinais principais:

- `STEP` — gera os impulsos de movimento;
- `DIR` — define o sentido de rotação.

No código, estes pinos foram definidos da seguinte forma:

```cpp
#define pinSTEP 9
#define pinDIR  10
```

---

## Wokwi

| Campo | Valor |
|-------|-------|
| **Link** | https://wokwi.com/projects/454346932045509633 |
| **Export local** | [wokwi/](wokwi/) |
| **Firmware simulação** | `wokwi/sketch.ino` |
| **Cópia no repo** | `elevador_v03_wokwi_mega_a4988_lcd.ino` (igual ao sketch) |
| **Biblioteca** | hd44780 (LCD I2C) |

### Simular no Cursor

1. Abrir pasta `wokwi/` como raiz do projeto Wokwi
2. Extensão **Wokwi Simulator** → `F1` → *Wokwi: Start Simulator*
