# Circuito v01 — Tinkercad com Arduino Uno, motor DC e LCD

Esta foi a primeira versão funcional do projeto do elevador de 4 pisos.  
Nesta fase, o sistema ainda se encontrava numa fase inicial de desenvolvimento e simulação no Tinkercad.

## Objetivo desta versão

O objetivo principal desta versão foi testar a lógica básica de funcionamento do elevador, incluindo:

- leitura dos botões exteriores;
- identificação do andar através de sensores simulados;
- controlo básico do movimento com motor DC;
- indicação visual dos pedidos através de LEDs;
- sinalização sonora com buzzer;
- apresentação do andar num LCD I2C;
- teste inicial da lógica de subida, descida e paragem.

## Componentes utilizados

- Arduino Uno R3
- Motor DC com caixa redutora
- Driver de motor / ponte H
- 4 botões de chamada exterior
- 4 LEDs verdes indicadores de pedidos
- 4 resistências de 1 kΩ
- 4 interruptores deslizantes para simular sensores de piso
- Buzzer piezoelétrico
- LCD 16x2 com comunicação I2C
- Regulador de tensão 5 V
- Fonte de alimentação externa
- Condensador de 100 nF
- Condensador eletrolítico de 470 µF
- Breadboards e cablagem

## Funcionamento geral

O utilizador seleciona um andar através dos botões exteriores.  
O sistema compara o andar atual com o andar de destino e decide se o elevador deve subir, descer ou parar.

A posição do elevador é simulada através de interruptores deslizantes, que representam sensores de fim de curso ou sensores de efeito Hall colocados nos pisos.

Quando o elevador chega ao andar de destino, o motor é parado, o LED correspondente é desligado e o buzzer emite um sinal sonoro curto.

O LCD apresenta a indicação dos andares e um ponteiro que representa a posição atual do elevador.

## Limitações desta versão

Esta versão ainda apresenta várias limitações:

- utiliza Arduino Uno, que tem menos pinos disponíveis;
- utiliza motor DC, sem controlo preciso de posição;
- não possui fila de pedidos;
- apenas guarda o último andar selecionado;
- não possui botões interiores de cabine;
- não possui lógica de emergência;
- não possui botão de reset;
- não possui sistema de rearme;
- não possui permissiva de porta;
- ainda não utiliza Arduino Mega;
- ainda não utiliza motor de passo;
- ainda não utiliza displays OLED.

## Importância desta fase

Apesar de ser uma versão inicial, esta fase foi importante porque permitiu testar a lógica base do elevador: leitura de botões, identificação de piso, comando do motor, sinalização por LEDs, buzzer e apresentação no LCD.

Esta versão serviu como ponto de partida para as versões seguintes do projeto.

---

## Etapa do projeto

Documentação completa: [E01 — Tinkercad v01](../../../../docs/ETAPAS/relatorios/E01_tinkercad_v01.md)

## Imagens

| Ficheiro | Descrição |
|----------|-----------|
| [imagens/tinkercad_anotado.png](imagens/tinkercad_anotado.png) | Circuito com anotações |
| [imagens/tinkercad_sem_anotacoes.png](imagens/tinkercad_sem_anotacoes.png) | Circuito sem anotações |
| [imagens/circuito.png](imagens/circuito.png) | Vista do circuito |
| [imagens/lista_componentes.png](imagens/lista_componentes.png) | Lista de componentes |

## Documentação

- [documentacao/Elevador_PaP.pdf](documentacao/Elevador_PaP.pdf)
