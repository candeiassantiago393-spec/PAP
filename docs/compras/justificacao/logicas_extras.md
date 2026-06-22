# Justificação — Lógicas Extras do Prédio (E12)

Componentes para **subsistemas do prédio inteligente** — independentes do elevador principal, mas integrados no conceito global da PAP.

---

## Módulo NodeMCU ESP32 USB-C (095-4900)

| | |
|---|---|
| **Função** | **Microcontrolador com Wi-Fi e Bluetooth** para módulos do prédio que beneficiam de comunicação sem fios (supervisão, integração futura com app, sensores remotos). |
| **Subsistema** | Plataforma base para várias lógicas extras |
| **Etapa** | E12 |
| **Fatura** | [2026EC1403323](../faturas_mauser/2026EC1403323.md) — 9,39 € |

---

## Teclado matricial de membrana 3×4 (096-4642)

| | |
|---|---|
| **Função** | **Entrada de código numérico** para acesso à porta do prédio — o utilizador pressiona combinação de teclas para pedir abertura. |
| **Subsistema** | [porta_predio](../../../logicas_extras/porta_predio/) · protótipo [cofre](../../../logicas_extras/cofre/) |
| **Etapa** | E12 |
| **Fatura** | [2026EC1403323](../faturas_mauser/2026EC1403323.md) — 4,07 € |

---

## Ecrã OLED 0,96" 128×64 I2C (096-7806)

| | |
|---|---|
| **Função** | **Display de protótipo** para testar mensagens e estados dos módulos extras antes de integrar displays maiores ou partilhar OLEDs do elevador. |
| **Subsistema** | Prototipagem geral / painéis de controlo |
| **Etapa** | E12 |
| **Fatura** | [2026EC1403323](../faturas_mauser/2026EC1403323.md) — 3,83 € |

---

## Placa protótipo photoshield (096-7946)

| | |
|---|---|
| **Função** | **Breadboard fixa** sobre Arduino Uno — montagem rápida de circuitos de teste para RFID, teclado e outros módulos sem soldar. |
| **Subsistema** | Bancada de desenvolvimento das lógicas extras |
| **Etapa** | E12 |
| **Fatura** | [2026EC1403323](../faturas_mauser/2026EC1403323.md) — 2,36 € |

---

## Módulo leitor RFID RC522 13,56 MHz (096-8517)

| | |
|---|---|
| **Função** | **Identificar cartões RFID** — cada residente pode ter cartão que abre a porta do prédio e/ou a porta do apartamento no respectivo andar. |
| **Subsistema** | [rfid](../../../logicas_extras/rfid/) |
| **Etapa** | E12 |
| **Fatura** | [2026EC1403323](../faturas_mauser/2026EC1403323.md) — 1,59 € |

---

## Placa LILYGO T-Camera S3 — ESP32 + câmara 5 MP + PIR (095-5977)

| | |
|---|---|
| **Função** | **Vigilância do prédio**: captura de imagem (OV5640 5 MP) e deteção de movimento (sensor PIR) para alarme ou registo de presença na entrada/zona comum. |
| **Subsistema** | [seguranca](../../../logicas_extras/seguranca/) |
| **Etapa** | E12 |
| **Fatura** | [2026EC1409725](../faturas_mauser/2026EC1409725.md) — 31,46 € |

---

## Caixa protetora T-Camera S3 (095-5979)

| | |
|---|---|
| **Função** | **Proteger mecanicamente** o módulo T-Camera na montagem final — isolamento e fixação na estrutura da maquete do prédio. |
| **Subsistema** | Segurança |
| **Etapa** | E12 |
| **Fatura** | [2026EC1409725](../faturas_mauser/2026EC1409725.md) — 3,29 € |

---

## Cabo PH2.0 → DuPont fêmea (095-5980)

| | |
|---|---|
| **Função** | **Interligar** o módulo T-Camera às breadboards ou ao ESP32 com conector PH2.0 de um lado e pinos DuPont do outro. |
| **Subsistema** | Segurança / ligação periféricos |
| **Etapa** | E12 |
| **Fatura** | [2026EC1409725](../faturas_mauser/2026EC1409725.md) — 0,77 € |

---

## Lógicas extras — componentes futuros (sem fatura ainda)

| Módulo | Componentes previstos | Função |
|--------|----------------------|--------|
| Meteorologia | Sensores temp/humidade | Dados ambientais do prédio |
| Toldo automático | Sensor de chuva | Acionar estendal quando chove |
| Painéis controlo | Displays, botões | Supervisão centralizada |
