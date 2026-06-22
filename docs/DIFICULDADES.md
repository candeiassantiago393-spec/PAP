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
- **Problema:** Projetos Wokwi existem só na conta online; repo sem `diagram.json` / `wokwi.toml`.
- **Causa:** Desenvolvimento feito no browser, sem integração local.
- **Solução:** Pendente — exportar projetos e colocar em `elevador/simulacao/wokwi/exports/`.
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
