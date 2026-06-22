# Contexto do Projeto — PAP Prédio Inteligente

> Documento vivo. Atualizado na Fase 0 de reorganização (junho 2026).

---

## 1. Identificação

| Campo | Valor |
|-------|-------|
| Projeto | Elevador PAP / Prédio Inteligente |
| Curso | Técnico de Eletrónica, Automação e Comando |
| Escola | Escola Secundária Emídio Navarro |
| Turma | 12.º PEAC |
| Ano letivo | 2025/2026 |
| Orientadores | Ricardo Feio, Manuel Queiroz, Rui Barros |
| Microcontrolador | Arduino Mega |
| Simulação | Tinkercad (fase inicial) → Wokwi (fase atual) |
| Implementação real | Maquete física com L298N, Hall, OLED SH1106 |

---

## 2. Conceito do sistema

### 2.1 Elevador (lógica principal)

Protótipo funcional de elevador de **4 pisos** com:

- Pedidos exteriores e interiores numa fila comum
- Priorização por sentido (subir/desce)
- Motor de passo (A4988 na simulação, L298N na realidade)
- 4 displays OLED (SSD1306 sim / SH1106 real) via TCA9548A
- Sensores de piso (interruptores sim / Hall real)
- Permissiva de porta, buzzer, reset, emergência e rearme
- Estados: NORMAL, PORTA, PARAGEM, ESTAB, ERRO_MOV, ERRO_PAR, REARME

### 2.2 Lógicas extras (prédio inteligente)

Subsistemas planeados ou em versão inicial — **foco secundário nesta fase**:

| Módulo | Descrição | Estado |
|--------|-----------|--------|
| Segurança | Sistemas de alarme/proteção do prédio | PIR no painel ecra |
| Painéis de controlo | Interfaces de supervisão | [Wokwi ESP32](../../logicas_extras/paineis_controlo/simulacao/wokwi/) |
| Meteorologia | Sensores ambientais gerais | No painel ecra (DHT22) |
| Porta do prédio | Código de acesso à entrada | Inicial |
| Cofre / código secreto | Acesso por PIN (teclado + servo) | [Simulação Wokwi](../../logicas_extras/cofre/) |
| RFID | Cartões por andar (porta prédio + apartamento) | Inicial |
| Toldo automático | Estendal por deteção de chuva | Inicial |

---

## 3. Evolução do código — simulação

### Linha temporal

```text
v01 ──► v02 ──► v03 ──► v04 ──► v05 ──► v06
Tinkercad   Tinkercad   Wokwi      Wokwi      Wokwi      Wokwi
Uno+DC      Uno+fila    Mega+A4988 7seg+erro  OLED×4     INTERIOR
                                              TCA9548A   +PORTA
```

### Detalhe por versão

| Versão | Plataforma | MCU | Motor | Interface | Marco |
|--------|------------|-----|-------|-----------|-------|
| **v01** | Tinkercad | Uno | DC + ponte H | LCD 16×2 | Lógica básica subir/descer |
| **v02** | Tinkercad | Uno | DC | LCD | Array `pedidos[4]` — fila |
| **v03** | Wokwi | Mega | Stepper A4988 | LCD 20×4 | Transição Mega, STEP/DIR |
| **v04** | Wokwi | Mega | A4988 | LCD + 7 segmentos | Reset, timeout, rearme |
| **v05** | Wokwi | Mega | A4988 | 4× OLED TCA9548A | Interface visual final sim |
| **v06** | Wokwi | Mega | A4988 | 4× OLED | **Referência sim** — interior, porta |

**Origem legada:** `Elevador-Projeto-Final-Curso-PAP-/circuitos/`  
**Destino:** `elevador/simulacao/legacy/`

---

## 4. Implementação real

| Aspeto | Simulação (v06) | Maquete real |
|--------|-----------------|--------------|
| Driver motor | A4988 | L298N |
| Display | SSD1306 | SH1106 |
| Sensor piso | Interruptor | Hall |
| Alimentação | USB sim | Fonte 12 V / 5 A industrial |

**Origem legada:** `Elevador-Projeto-Final-Curso-PAP-/circuito_real/`  
**Destino:** `elevador/real/l298n_sh1106_hall/`

---

## 5. Testes de bancada

Testes isolados de hardware antes da integração na maquete.

| Teste | Ficheiro legado | Destino |
|-------|-----------------|---------|
| Botão + LED latched | `Test_01_External_Requests_Button_LED_Latched.ino` | `elevador/testes/bancada/pedidos_exteriores/` |
| Botões only | `Test_02_External_Buttons_Only.ino` | `elevador/testes/bancada/pedidos_exteriores/` |
| OLED bench | `oledbecnch.ino` | `elevador/testes/bancada/oled/` |
| Sensor Hall | `sensorhall.ino` | `elevador/testes/bancada/sensores_hall/` |

**Origem legada:** `Elevador-Projeto-Final-Curso-PAP-/Test Bench PAP/`  
**Estado:** Componentes validados em bancada; a integrar na maquete.

---

## 6. Fase atual — maquete física

### Pipeline de validação

```text
Programar/Projetar → Simulação virtual → Teste bancada → Montagem maquete
        ✓                  ✓                  ✓              EM CURSO
```

### Progresso de montagem (junho 2026)

- [x] Botões exteriores de pedido
- [x] LEDs exteriores de pedido
- [x] Displays OLED
- [ ] Sensores Hall (próximo)
- [ ] Motor de passo + L298N (próximo)
- [ ] Botões interiores
- [ ] LEDs interiores
- [ ] Permissiva de porta
- [ ] Buzzer, reset
- [x] Estrutura madeira + cabine (recipiente adaptado)
- [ ] Tambor + cabo de aço

Ver [elevador/maquete/ESTADO_ATUAL.md](../elevador/maquete/ESTADO_ATUAL.md) e vídeo em `elevador/maquete/imagens/videos/`.

---

## 7. Pinagem principal (Mega — v06)

| Função | Pino |
|--------|------|
| Botão exterior piso 1–4 | D2–D5 |
| Buzzer | D6 |
| Reset | D7 |
| Sensor piso 1 | D8 |
| STEP (A4988) / L298N | D9 |
| DIR | D10 |
| Sensor piso 2–4 | D11–D13 |
| Permissiva porta | D22 |
| Botão interior 1–4 | D23–D26 |
| LED interior 1–4 | D27–D30 |
| LED exterior 1–4 | A0–A3 |
| I2C SDA/SCL | D20/D21 |

---

## 8. Interface de organização

App desenvolvida pelo autor para gestão de tarefas, projetos e documentação do PAP.

- **Repo:** [geral-controll-app](https://github.com/candeiassantiago393-spec/geral-controll-app)
- **Deploy:** Render / GitHub Pages
- **Na apresentação:** demonstrar como hub de organização do trabalho no Cursor

---

## 9. Problemas conhecidos no repo legado

| Problema | Impacto |
|----------|---------|
| Pastas duplicadas (`circuito_real/circuito_real/`) | Confusão ao navegar |
| README descreve estrutura inexistente | Desalinhamento doc ↔ código |
| Sem `diagram.json` Wokwi no repo | Simulação só no browser |
| `Imagens reais` vazio | Sem registo visual da maquete |
| Nomes com espaços (`Test Bench PAP`) | Problemas em scripts/CI |

---

## 10. Plano de migração

| Commit | Conteúdo | Estado |
|--------|----------|--------|
| 1 | `docs: contexto inicial e topologia` | **Este commit** |
| 2 | `docs: migrar relatórios PDF/DOCX` | Concluído |
| 3 | `feat(elevador): legacy tinkercad v01-v02` | Concluído |
| 4 | `feat(elevador): legacy wokwi v03-v06` | Concluído |
| 5 | `feat(elevador): implementação real L298N` | Concluído |
| 6 | `test(elevador): bancada hardware` | Concluído |
| 7 | `chore(elevador): integrar exports Wokwi v03–v06` | Concluído |
| 8 | `chore: PlatformIO firmware real` | Concluído |
| 9 | `docs: etapas, compras e justificações` | Pendente commit |
| 8+ | `feat(logicas_extras): módulos` | Futuro |

---

## 11. Pendências

- [x] Exportar `diagram.json` de cada projeto Wokwi
- [x] Migrar relatórios PDF/DOCX → `docs/relatorios/`
- [x] Migrar firmware real L298N → `elevador/real/l298n_sh1106_hall/`
- [x] Configurar PlatformIO para compilação no Cursor
- [ ] Migrar fotos adicionais da maquete
- [ ] Esquemas KiCad/Fritzing (se existirem)
- [ ] Fotos estrutura mecânica → `elevador/hardware/mecanica/imagens/`
- [ ] Apresentação 3D interativa (fase posterior)
