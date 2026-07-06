# Cartões de Prédio — RFID

Acesso por **cartão RFID** à porta do prédio e aos apartamentos por andar.

| Campo | Valor |
|-------|-------|
| **Estado** | Inicial — hardware comprado, firmware por desenvolver |
| **MCU planeado** | ESP32 (NodeMCU USB-C) |
| **Leitor** | RC522 13,56 MHz |
| **Etapa** | [E12 — Lógicas extras](../../docs/ETAPAS/relatorios/E12_logicas_extras.md) |

---

## Objectivo na maquete

Cada cartão tem um UID único. O sistema valida se o cartão tem permissão para o **hall do prédio** ou para um **andar/apartamento** específico e aciona a fechadura (relé ou servo).

---

## Hardware comprado

| Componente | Código Mauser | Fatura |
|------------|---------------|--------|
| Leitor RFID RC522 13,56 MHz | 096-8517 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |
| NodeMCU ESP32 USB-C | 095-4900 | [2026EC1403323](../../docs/compras/faturas_mauser/2026EC1403323.md) |

---

## Estrutura

| Pasta | Conteúdo |
|-------|----------|
| [documentacao/](documentacao/) | Funcionamento, pinagem e mapa de cartões |
| [simulacao/](simulacao/) | Mock / simulação (por criar) |
| [real/](real/) | Firmware RC522 + montagem física — **pendente** |
| [testes/](testes/) | Leitura UID, autorização — **pendente** |

---

## Relacionado

| Módulo | Ligação |
|--------|---------|
| Código porta prédio | [../cofre/](../cofre/) — acesso alternativo por PIN |
| Central de controlo | [../central_de_controlo/](../central_de_controlo/) — alarmes e ambiente |

---

## Próximos passos

- [ ] Ler UID de cartões de teste com RC522
- [ ] Definir lista de cartões autorizados (por andar)
- [ ] Simular leitura antes da montagem na maquete
- [ ] Integrar com fechadura / relé na porta

## Navegação

| | |
|--|--|
| Lógicas extras | [../README.md](../README.md) |
| Índice repo | [../../docs/INDICE_REPOSITORIO.md](../../docs/INDICE_REPOSITORIO.md) |
