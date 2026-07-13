# Teste PIR — movimento (Serial)

Validação isolada do **sensor PIR HC-SR501** — só Serial Monitor, sem OLED.

## Pinagem (Arduino Mega)

| PIR | Mega | Função |
|-----|------|--------|
| **VCC** | **5V** | Alimentação |
| **GND** | **GND** | Massa |
| **AL** (preto, direita) | **pin 5** | **HIGH** = movimento detetado |
| **meio** (branco) | **GND** | Massa |

```
    PIR mini (teu modulo)     MEGA
    --------------------     ----
    +  vermelho (esq)  ---- 5V
    ?  branco (meio)   ---- GND
    AL preto (dir)    ---- D5 (digital pin 5)
```

> Pin 5 = `PIN_PIR` no firmware completo da Central de Controlo (Mega).

## Upload

```powershell
cd logicas_extras/central_de_controlo/testes/bancada/pir_serial
pio run -t upload --upload-port COM7
pio device monitor -b 9600
```

**Fecha** o dashboard / Serial Monitor antes do upload.

## Serial

```
HIGH   | 1         >>> MOVIMENTO <<<
LOW    | -            sem movimento
HIGH   | 2         >>> MOVIMENTO <<<
```

| Campo | Significado |
|-------|-------------|
| `HIGH` | PIR detetou movimento |
| `LOW` | Sem movimento |
| `deteccoes` | Contador de activações (flanco LOW→HIGH) |

## Montagem

1. Liga **VCC, GND, OUT** conforme tabela
2. Liga o Mega por USB e faz upload
3. Abre Serial Monitor a **9600 baud**
4. **Espera 30 s** — o PIR precisa de aquecer (o código avisa)
5. Passa a mão a ~1–3 m do sensor — deve aparecer `>>> MOVIMENTO <<<`

## Se não funcionar

| Sintoma | Verificar |
|---------|-----------|
| Sempre `HIGH` | Sensor demasiado sensível — afasta ou reduz tempo no pot do módulo |
| Nunca `HIGH` | OUT mal ligado ao pin 5; VCC em 5V; esperaste os 30 s? |
| Valores instáveis | Evita sol directo / corrente de ar no sensor |

## Ajuste no módulo

O HC-SR501 tem dois potenciómetros:

- **Sensibilidade** — distância de deteção
- **Tempo** — quanto tempo o OUT fica HIGH após deteção

Para este teste básico, começa com os valores de fábrica.
