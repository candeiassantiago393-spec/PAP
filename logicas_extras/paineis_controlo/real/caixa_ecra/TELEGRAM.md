# Alertas Telegram — configurar e testar

O ESP32 envia mensagens quando:

- **PIR** deteta movimento (intruso)
- **Solo** passa a SECO (&lt; 35 %)

---

## 1. Criar o bot no Telegram

1. Abre o Telegram e procura **@BotFather**
2. Envia `/newbot`
3. Escolhe um nome e um username (tem de acabar em `bot`)
4. Copia o **token** (ex.: `7123456789:AAHxxxxxxxxxxxxxxxxxxxxxxxx`)

---

## 2. Obter o Chat ID

1. Procura o teu bot pelo username e envia qualquer mensagem (ex.: `ola`)
2. No browser (substitui `TOKEN` pelo token real):

   ```
   https://api.telegram.org/botTOKEN/getUpdates
   ```

3. Procura `"chat":{"id":123456789` — esse número é o **CHAT_ID**

---

## 3. Configurar o projeto

Na pasta do projeto:

```powershell
copy secrets.example.h secrets.h
```

Edita `secrets.h`:

```c
#define WIFI_SSID "Wokwi-GUEST"      // simulador Wokwi
#define WIFI_PASSWORD ""
#define TELEGRAM_BOT_TOKEN "7123..."
#define TELEGRAM_CHAT_ID "123456789"
#define TELEGRAM_TEST_ON_BOOT 1
```

> **Nunca** faças commit do `secrets.h` — já está no `.gitignore`.

---

## 4. Testar no Wokwi

1. Compila: `python -m platformio run`
2. Abre o projeto no Wokwi → **Stop + Start**
3. Abre o **Serial Monitor** (115200 baud)

Deves ver algo como:

```
Wi-Fi a ligar (Wokwi-GUEST)...
Wi-Fi OK: 10.x.x.x
Telegram: mensagem de teste enviada
```

Recebes no Telegram: *"candeias.dev: sistema online (teste)."*

### Testar intruso (PIR)

1. Clica no sensor **PIR** na simulação (ou ativa movimento)
2. Mensagem: *"candeias.dev: Intruso detetado!"*

### Testar solo seco

1. Roda o **potenciómetro** do solo para baixo (&lt; 35 % na barra)
2. Mensagem: *"candeias.dev: Solo SECO! Regar plantas."*
3. Só volta a avisar quando o solo subir acima de 35 % e descer outra vez

---

## 5. Testar no ESP32 real

1. Em `secrets.h`, põe o **SSID e password** da tua rede Wi‑Fi (**2.4 GHz**)
2. Mantém o mesmo `TELEGRAM_BOT_TOKEN` e `TELEGRAM_CHAT_ID`
3. `TELEGRAM_TEST_ON_BOOT` pode ficar a `1` na primeira vez, depois `0`
4. Grava o firmware: `python -m platformio run -t upload`
5. Monitor: `python -m platformio device monitor`

---

## Resolução de problemas

| Sintoma | Solução |
|--------|---------|
| `Telegram: desativado` | Token ou Chat ID vazio em `secrets.h` |
| `Wi-Fi falhou` | No Wokwi usa `Wokwi-GUEST`; em casa confirma SSID/password |
| `Telegram HTTP 401` | Token errado |
| `Telegram HTTP 400` | Chat ID errado — envia mensagem ao bot e volta a ler getUpdates |
| Sem mensagem no Wokwi | Reinicia simulação após `platformio run`; espera ~10 s no arranque |
| Muitas mensagens solo | Normal só na transição para SECO; sobe o potenciometro para resetar |

---

## Segurança

- O token do bot é como uma password — não partilhes nem publiques
- Se expuseres o token, revoga no @BotFather com `/revoke`
