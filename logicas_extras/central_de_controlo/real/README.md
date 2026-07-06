# Implementação real — Central de Controlo

Montagem física da caixa ESP32 (estilo MagSafe) na maquete do prédio.

## Estado

**Pendente** — simulação Wokwi funcional; montagem na caixa física por fazer.

## Checklist (montagem)

- [ ] ESP32 DevKit + OLED SSD1306 I2C
- [ ] DHT22, PIR, buzzer, 3× LED, bomba/relé
- [ ] Sensores do kit Mauser: gás (MQ), fogo (KY-026), vibração (SW-420)
- [ ] Sensor solo capacitivo (substituir potenciómetro)
- [ ] Botões NEXT e OK no painel
- [ ] Alimentação USB ou 5 V regulado

## Firmware

Usar o mesmo código de [../sketch.ino](../sketch.ino) / [../src/main.cpp](../src/main.cpp) — ajustar pinagem se necessário.

Documentação: [../documentacao/pinagem.md](../documentacao/pinagem.md)
