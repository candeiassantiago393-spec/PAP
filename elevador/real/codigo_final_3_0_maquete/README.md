# Código Final 3.0 — maquete

Pasta-mãe dos firmwares da **maquete física**. Cada subpasta é uma variante independente (PlatformIO + `src/main.ino` + README próprio).

## Variantes

| Pasta | Estado | Descrição |
|-------|--------|-----------|
| [ext_porta/](ext_porta/) | **Activo / testado** | Pedidos exteriores, porta D22, sem OLED/int. |
| _(futuro)_ | — | Ex.: `com_oled`, `com_pedidos_internos`, … |

Para adicionar uma variante nova:

1. Criar subpasta `nome_variant/`
2. Copiar `platformio.ini` de `ext_porta/`
3. Colocar firmware em `nome_variant/src/main.ino`
4. Escrever `nome_variant/README.md` com pinagem, estado de testes e upload
5. Registar a linha na tabela acima

## Upload rápido (variante actual)

```powershell
cd elevador/real/codigo_final_3_0_maquete/ext_porta
pio run -t upload
pio device monitor -b 9600
```

```powershell
cd elevador/testes/bancada
.\upload.ps1 maquete
```

## Diferenças face à versão completa

| Aspeto | [codigo_final_3_0](../codigo_final_3_0/) | Maquete (variantes aqui) |
|--------|------------------------------------------|---------------------------|
| OLED SH1106 / TCA9548A | Sim | Por variante (ext_porta: não) |
| Botões/LEDs interiores | Sim | Por variante (ext_porta: não) |
| Permissiva porta D22 | Não | ext_porta: sim |

## Navegação

| | |
|--|--|
| Real (índice) | [../README.md](../README.md) |
| Versão completa congelada | [../codigo_final_3_0/](../codigo_final_3_0/) |
| Testes bancada | [../../testes/bancada/](../../testes/bancada/) |
| Pinagem | [../../documentacao/pinagem.md](../../documentacao/pinagem.md) |
