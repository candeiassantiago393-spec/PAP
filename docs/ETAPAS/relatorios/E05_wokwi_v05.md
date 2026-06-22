# E05 — Wokwi v05 — OLED via TCA9548A

| Campo | Valor |
|-------|-------|
| **Estado** | Concluída |
| **Período** | Dezembro 2025 |
| **Código** | [elevador/simulacao/legacy/wokwi/v05_oled_tca9548a](../../../elevador/simulacao/legacy/wokwi/v05_oled_tca9548a/) |
| **Wokwi** | https://wokwi.com/projects/455660851315238913 |

## Objetivo

Substituir LCD e displays de 7 segmentos por **4 OLEDs SSD1306** ligados através do multiplexador I2C **TCA9548A**, com painel informativo completo.

## Motivação (evolução face à v04)

Após experimentar os 7 segmentos, a pesquisa no Wokwi mostrou que os **OLEDs** permitiam ir além da indicação do piso: apresentar **fila de pedidos, sentido, estado do elevador, emergência** e outras mensagens de supervisão — requisito identificado na v04.

## O que foi feito

- 4× OLED SSD1306 com o mesmo endereço I2C, separados por canais do TCA9548A
- Bibliotecas: Adafruit GFX + Adafruit SSD1306
- Apresentação por display: piso, sentido, fila, estado, pedido recente
- Mantida toda a lógica de segurança da v04 (reset, erro, rearme, A4988)

## Decisões técnicas

- TCA9548A para evitar conflito de endereços I2C idênticos
- Um OLED por zona informativa (modelo próximo da maquete final)
- Interface unificada em I2C (SDA/SCL no Mega)

## Dificuldades

- **Configuração do TCA9548A e dos canais I2C** — dificuldade moderada: selecção correcta do canal antes de cada `begin()`/escrita no OLED; atenção à ordem de inicialização dos quatro displays
- Resolvido com iteração e testes na simulação

## Resultado / validação

- Quatro displays independentes a funcionar na simulação
- Painel informativo rico — base visual para a v06 e para a maquete real

## Evidências

| Ficheiro | Descrição |
|----------|-----------|
| [wokwi/](../../../elevador/simulacao/legacy/wokwi/v05_oled_tca9548a/wokwi/) | Export completo |
| [imagem_circuito.png](../../../elevador/simulacao/legacy/wokwi/v05_oled_tca9548a/imagem_circuito.png) | Vista do circuito |

## Próximo passo

→ [E06 — pedidos interiores + porta](E06_wokwi_v06.md)
