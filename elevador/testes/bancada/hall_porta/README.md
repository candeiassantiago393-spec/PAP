# Teste — permissiva da porta (D22)

Validação da **chave deslizante** da porta na maquete.  
Lógica igual à simulação v06 — pino **D22**.

| Campo | Valor |
|-------|-------|
| Tipo | **Chave deslizante** (contacto seco) |
| Pino Mega | **D22** |
| Lógica firmware | **LOW** = porta **fechada** · **HIGH** = porta **aberta** |
| Pull-up | Interno (`INPUT_PULLUP`) |

## Lógica

| Sensor | Pinos | Activo → |
|--------|-------|----------|
| Pisos (A3144) | D7, D8, D11, D12 | **LOW** (cabine no piso) |
| **Porta (chave)** | **D22** | **LOW** (porta **fechada**) |

## Cablagem chave deslizante

```
Terminal 1 da chave ── GND (Mega)
Terminal 2 da chave ── D22

Fechada  → D22 ligado a GND → LOW  → PORTA FECHADA
Aberta   → D22 flutuante (pull-up) → HIGH → PORTA ABERTA
```

Na bancada (sem chave): ligar **D22→GND** simula porta fechada.

## Upload e monitor

```powershell
cd elevador/testes/bancada
.\upload.ps1 hall_porta
pio device monitor -d hall_porta
```

## Critérios de sucesso

- [ ] Chave fechada / D22→GND: Serial `PORTA FECHADA` (D22=LOW)
- [ ] Chave aberta: Serial `PORTA ABERTA` (D22=HIGH)

## Referências

| | |
|--|--|
| Pinagem maquete | [pinagem.md](../../../documentacao/pinagem.md) |
| Teste Hall pisos | [../sensores_hall/](../sensores_hall/) |
