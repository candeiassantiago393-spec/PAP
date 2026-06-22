#pragma once

// Copia este ficheiro para "secrets.h" (na mesma pasta) e preenche os valores.
// O secrets.h NAO vai para o GitHub.

// --- Wi-Fi ---
// Wokwi (simulador): SSID "Wokwi-GUEST", password vazio
// Casa: nome e password da tua rede 2.4 GHz
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// --- Telegram ---
// 1) Telegram -> @BotFather -> /newbot -> copia o token
// 2) Envia uma mensagem ao teu bot
// 3) Abre no browser (substitui TOKEN):
//    https://api.telegram.org/botTOKEN/getUpdates
// 4) Copia o "chat":{"id": NUMERO } para baixo
#define TELEGRAM_BOT_TOKEN ""
#define TELEGRAM_CHAT_ID ""

// Mensagem de teste ao arrancar (0 = desligado, 1 = ligado)
#define TELEGRAM_TEST_ON_BOOT 1
