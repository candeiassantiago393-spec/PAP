# Funcionamento — Cartões de Prédio (RFID)

## Conceito

```text
Cartão aproximado ──► RC522 lê UID ──► ESP32 compara com lista autorizada
                                              ├─ autorizado → abre fechadura (relé/servo)
                                              └─ negado      → LED/buzzer de erro
```

## Níveis de acesso (planeado)

| Tipo de cartão | Permissão |
|----------------|-----------|
| **Admin / porteiro** | Hall + todos os andares |
| **Morador andar N** | Hall + apartamento do andar N |
| **Visitante** | Hall apenas (tempo limitado) |

## Fluxo detalhado (v1 — por implementar)

1. ESP32 inicializa SPI + RC522.
2. Loop: aguarda cartão (`PICC_IsNewCardPresent`).
3. Lê UID (4 ou 7 bytes conforme cartão).
4. Procura UID na tabela em memória (ou EEPROM).
5. Se válido: activa saída digital (relé) durante X segundos.
6. Se inválido: sinal sonoro/visual de negação.
7. Cooldown entre leituras para evitar duplicados.

## Mapa de cartões (exemplo — preencher na montagem)

| UID (hex) | Titular | Andar | Notas |
|-----------|---------|-------|-------|
| _a definir_ | — | — | Ler cartões reais na bancada |

## Melhorias futuras

- Registo de novos cartões via modo admin
- Log de acessos (Serial ou Wi‑Fi)
- Integração com central de controlo (alerta intruso se cartão inválido repetido)
