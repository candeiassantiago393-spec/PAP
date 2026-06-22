# Funcionamento — Meteorologia (via painel ecra)

> A meteorologia do prédio está implementada nos ecrãs **AMBIENTE** e **GRÁFICO** do projeto [paineis_controlo](../../paineis_controlo/).  
> Firmware: [caixa_ecra/src/main.cpp](../../paineis_controlo/real/caixa_ecra/src/main.cpp)

## Objectivo no prédio inteligente

Cada **andar** deve ter um **ecra** no hall que mostra condições ambientais (temperatura, humidade) e evolução recente da temperatura. Na apresentação PAP usa-se **uma caixa demo** com o mesmo firmware; no futuro replica-se por piso.

## Sensor

| Campo | Valor |
|-------|-------|
| Modelo | **DHT22** (temperatura + humidade) |
| Biblioteca | `DHTesp` (PlatformIO / Wokwi) |
| GPIO | **26** |
| Intervalo leitura | **2000 ms** (`updateEnvironmentData`) |

### Valores no ecrã AMBIENTE

- `Temperatura: XX.X C`
- `Humidade:    XX.X %`
- Enquanto o DHT inicializa: mensagem *"A ler DHT22... Aguarde 2s"*

## Ecrã GRÁFICO (sparkline)

| Parâmetro | Valor |
|-----------|-------|
| Amostras guardadas | **48** (`SPARK_LEN`) |
| Área do gráfico | 120×42 px no OLED |
| Conteúdo | Curva de temperatura (min/max auto-escala) |
| Estado inicial | *"A recolher..."* até haver ≥ 2 amostras |

Cada leitura válida de temperatura chama `pushTempSample()` — o gráfico reflecte ~**1,5 min** de histórico contínuo (48 × 2 s).

## Navegação no painel

O utilizador chega aos ecrã meteorológicos com o botão **NEXT (GPIO 32)**:

```text
HOME → AMBIENTE → MOVIMENTO → PLANTAS → GRÁFICO → Instagram → (ciclo)
         ↑ meteorologia    ↑ segurança              ↑ meteorologia
```

**Modo feira:** rotação automática a cada **5 s** entre todos os ecrãs.

## Relação com outros módulos no mesmo ESP32

O painel ecra agrega várias lógicas extras num único OLED:

| Ecrã | Módulo PAP |
|------|------------|
| AMBIENTE, GRÁFICO | **Meteorologia** (este) |
| MOVIMENTO | [Segurança](../../seguranca/) |
| PLANTAS | Estufa / rega (referência Projeto-Estufa) |

Documentação completa do painel: [paineis_controlo/documentacao/funcionamento.md](../../paineis_controlo/documentacao/funcionamento.md).

## Evolução — um ecra por andar

| Passo | Descrição |
|-------|-----------|
| 1 | Validar DHT22 + ecrãs na caixa demo (actual) |
| 2 | Montar caixa num andar da maquete |
| 3 | Replicar ESP32 + OLED + DHT22 nos restantes andares |
| 4 | _(opcional)_ Endereços I2C distintos ou MCUs separados por piso |

## Simular

1. Abrir [paineis_controlo/simulacao/wokwi/](../../paineis_controlo/simulacao/wokwi/)
2. Wokwi: Start Simulator
3. Botão NEXT até **AMBIENTE** ou **GRÁFICO**

## Navegação

| | |
|--|--|
| Módulo meteorologia | [../README.md](../README.md) |
| Pinagem DHT | [pinagem.md](pinagem.md) |
| Painel ecra | [../../paineis_controlo/](../../paineis_controlo/) |
