# Registo de Dificuldades — PAP

> Documento vivo. Cada entrada regista um problema encontrado, o contexto e a resolução (ou estado).

---

## Como usar este ficheiro

```text
### [DATA] Título curto
- **Área:** elevador | logicas_extras | maquete | git | ferramentas
- **Fase:** simulação | bancada | maquete | documentação
- **Problema:** descrição
- **Causa:** (se conhecida)
- **Solução:** (ou "em aberto")
- **Lição:** o que evitar no futuro
```

---

## Entradas

### [2026-06] Estrutura de pastas desorganizada no repo legado
- **Área:** git
- **Fase:** documentação
- **Problema:** O repositório `Elevador-Projeto-Final-Curso-PAP-` tinha pastas aninhadas por engano (`circuito_real/circuito_real/`, `v04/circuitos/v04/`), nomes com espaços e README que descrevia uma estrutura diferente da real.
- **Causa:** Uploads incrementais via interface web do GitHub, sem convenção de pastas definida no início.
- **Solução:** Criação do repo `PAP` com topologia segmentada; migração faseada com commits documentados.
- **Lição:** Definir estrutura antes do primeiro commit de código.

---

### [2026-06] Arduino Uno insuficiente para expansão
- **Área:** elevador
- **Fase:** simulação
- **Problema:** v01–v02 no Tinkercad com Uno ficaram sem pinos para botões interiores, porta, reset e mais LEDs.
- **Causa:** Limitação de GPIO do ATmega328P.
- **Solução:** Migração para Arduino Mega no Wokwi (v03+).
- **Lição:** Dimensionar MCU desde o início quando o sistema tem I/O previsto para crescer.

---

### [2026-06] Tinkercad sem Mega adequado
- **Área:** elevador
- **Fase:** simulação
- **Problema:** Necessidade de Mega mas Tinkercad não suportava a evolução pretendida.
- **Causa:** Limitação da plataforma Tinkercad.
- **Solução:** Transição para Wokwi a partir da v03.
- **Lição:** Escolher simulador que suporte o hardware alvo.

---

### [2026-06] Displays de 7 segmentos abandonados
- **Área:** elevador
- **Fase:** simulação
- **Problema:** v04 testou 7 segmentos para indicar piso; solução limitada em informação e pinos.
- **Causa:** Experimentação de interface visual.
- **Solução:** v05 adoptou 4× OLED via multiplexador TCA9548A.
- **Lição:** OLEDs permitem mais estados (fila, sentido, erro) num único hardware.

---

### [2026-06] Diferença simulação vs real — driver motor
- **Área:** elevador
- **Fase:** maquete
- **Problema:** Simulação usa A4988; maquete real usa L298N com motor bipolar NEMA 17.
- **Causa:** A4988 adequado ao Wokwi; L298N disponível e testado para montagem física.
- **Solução:** Código adaptado em `circuito_real/`; pinagem STEP/DIR reutilizada onde possível.
- **Lição:** Manter ramos `simulacao/` e `real/` separados com README de diferenças.

---

### [2026-06] OLED SSD1306 vs SH1106
- **Área:** elevador
- **Fase:** bancada / maquete
- **Problema:** Displays reais SH1106 podem não renderizar com biblioteca/config SSD1306 da simulação.
- **Causa:** Offset de memória diferente no controlador SH1106.
- **Solução:** Teste dedicado `oledbecnch.ino`; driver/biblioteca ajustada no código real.
- **Lição:** Validar cada componente real em bancada antes da maquete.

---

### [2026-06] Sem exports Wokwi no repositório
- **Área:** ferramentas
- **Fase:** simulação
- **Problema:** Projetos Wokwi existiam só na conta online; repo sem `diagram.json` / `wokwi.toml`.
- **Causa:** Desenvolvimento feito no browser, sem integração local.
- **Solução:** Exports v03–v06 integrados em `elevador/simulacao/legacy/wokwi/*/wokwi/`.
- **Lição:** Versionar exports Wokwi junto com cada `.ino`.

---

### [2026-06] Conta Wokwi indisponível no PC da empresa
- **Área:** ferramentas
- **Fase:** documentação
- **Problema:** Impossível obter links/exports Wokwi durante reorganização no trabalho.
- **Causa:** Conta pessoal Wokwi só em casa.
- **Solução:** Adiar para sessão em casa; estrutura de pastas já preparada.
- **Lição:** Manter links Wokwi no README de cada versão legacy.

---

### [2025-11] Indefinição sobre plataforma de simulação do Arduino Mega
- **Área:** elevador
- **Fase:** simulação
- **Problema:** Após a v02, era necessário migrar para o Mega, mas ainda não estava escolhida uma ferramenta de simulação adequada.
- **Causa:** Limitações do Tinkercad para o Arduino Mega; ausência de alternativa definida no momento.
- **Solução:** O orientador Rui Barros identificou o Wokwi e comunicou a opção ao autor (novembro 2025). A migração foi estável.
- **Lição:** Reportar bloqueios técnicos aos orientadores atempadamente; o Wokwi tornou-se a plataforma central do projeto.

---

### [2026-02] Atraso na entrega da fonte de alimentação
- **Área:** elevador
- **Fase:** bancada
- **Problema:** Fonte industrial 12 V demorou a chegar; projecto encravou temporariamente nos testes integrados com motor.
- **Causa:** Prazo de entrega do fornecedor (Mauser).
- **Solução:** Testes retomados após recepção (fevereiro–março 2026).
- **Lição:** Encomendar componentes críticos de potência com antecedência; documentar em `docs/compras/`.

---

### [2026-06] I2C dos OLEDs a causar gaguejar do motor (bancada)
- **Área:** elevador
- **Fase:** bancada
- **Problema:** 4 OLEDs no mesmo SDA/SCL; motor de passo gaguejava durante testes.
- **Causa provável:** interferência no barramento I2C e/ou picos de consumo durante refresh dos displays.
- **Solução:** Refactor do código (sugestão externa); versão actual no firmware real validada em bancada.
- **Lição:** Separar timing de refresh I2C do controlo do motor; validar sempre com carga real ligada.

---

### [2026-06] Maus contactos e cabos curtos na maquete
- **Área:** elevador
- **Fase:** maquete
- **Problema:** Falhas intermitentes por mau contacto; cabos demasiado curtos para a estrutura.
- **Causa:** Cablagem inicial inadequada em comprimento e terminações.
- **Solução:** Compra de rolos de jumpers e ponteiras (ferrules) para ligações mais fiáveis.
- **Lição:** Prever cablagem e terminações desde o início da montagem física; documentar em `docs/compras/`.

---

### [2026-06-22] Botão / LED do 3.º andar exterior não responde
- **Área:** elevador
- **Fase:** maquete
- **Problema:** Nos testes dos 4 botões + LEDs exteriores, o **piso 3** não funciona correctamente; os restantes andares OK.
- **Causa:** Suspeita de **mau contacto** ou ligação — a confirmar com testes de continuidade.
- **Solução:** Em aberto — rever soldadura, ponteiras e cabo até D4/A2 (botão/LED piso 3).
- **Lição:** Testar cada par botão+LED isoladamente antes de fechar cablagem no shaft.
- **Evidência:** [vídeo 2026-06-22](../elevador/maquete/imagens/videos/2026-06-22_maquete_botoes_leds_exteriores.mp4)

---

## Template para novas entradas

```markdown
### [AAAA-MM-DD] Título
- **Área:**
- **Fase:**
- **Problema:**
- **Causa:**
- **Solução:**
- **Lição:**
```
