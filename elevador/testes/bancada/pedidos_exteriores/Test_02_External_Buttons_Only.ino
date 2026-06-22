/*
  Test_02_External_Buttons_Only.ino

  Objetivo:
  Testar apenas os botões exteriores de chamada do elevador.

  Funcionamento:
  - Carregar no botão exterior do piso 1 -> Serial Monitor mostra "Button 1 pressed"
  - Carregar no botão exterior do piso 2 -> Serial Monitor mostra "Button 2 pressed"
  - Carregar no botão exterior do piso 3 -> Serial Monitor mostra "Button 3 pressed"
  - Carregar no botão exterior do piso 4 -> Serial Monitor mostra "Button 4 pressed"

  Hardware:
  - Arduino Mega
  - Botões ligados entre o pino de entrada do Arduino e GND

  Lógica:
  - É usado INPUT_PULLUP
  - Botão solto     = HIGH
  - Botão carregado = LOW
*/

// ================= PINAGEM DOS BOTÕES EXTERIORES =================

// Botão exterior do piso 1 ligado ao pino digital 2
#define pinBotaoExt1 2

// Botão exterior do piso 2 ligado ao pino digital 3
#define pinBotaoExt2 3

// Botão exterior do piso 3 ligado ao pino digital 4
#define pinBotaoExt3 4

// Botão exterior do piso 4 ligado ao pino digital 5
#define pinBotaoExt4 5


// ================= ESTADOS ANTERIORES DOS BOTÕES =================

// Guarda o estado anterior do botão 1
bool ultimoBotaoExt1 = HIGH;

// Guarda o estado anterior do botão 2
bool ultimoBotaoExt2 = HIGH;

// Guarda o estado anterior do botão 3
bool ultimoBotaoExt3 = HIGH;

// Guarda o estado anterior do botão 4
bool ultimoBotaoExt4 = HIGH;


void setup() {
  // Inicia a comunicação série
  Serial.begin(9600);

  // Configura o botão exterior do piso 1 com resistência interna pull-up
  pinMode(pinBotaoExt1, INPUT_PULLUP);

  // Configura o botão exterior do piso 2 com resistência interna pull-up
  pinMode(pinBotaoExt2, INPUT_PULLUP);

  // Configura o botão exterior do piso 3 com resistência interna pull-up
  pinMode(pinBotaoExt3, INPUT_PULLUP);

  // Configura o botão exterior do piso 4 com resistência interna pull-up
  pinMode(pinBotaoExt4, INPUT_PULLUP);

  // Mensagem inicial
  Serial.println("Teste apenas dos botoes exteriores iniciado.");
}


void loop() {
  // Lê o estado atual do botão 1
  bool botaoAtualExt1 = digitalRead(pinBotaoExt1);

  // Lê o estado atual do botão 2
  bool botaoAtualExt2 = digitalRead(pinBotaoExt2);

  // Lê o estado atual do botão 3
  bool botaoAtualExt3 = digitalRead(pinBotaoExt3);

  // Lê o estado atual do botão 4
  bool botaoAtualExt4 = digitalRead(pinBotaoExt4);


  // Deteta a transição do botão 1: HIGH para LOW
  if (ultimoBotaoExt1 == HIGH && botaoAtualExt1 == LOW) {
    Serial.println("Button 1 pressed");
  }

  // Deteta a transição do botão 2: HIGH para LOW
  if (ultimoBotaoExt2 == HIGH && botaoAtualExt2 == LOW) {
    Serial.println("Button 2 pressed");
  }

  // Deteta a transição do botão 3: HIGH para LOW
  if (ultimoBotaoExt3 == HIGH && botaoAtualExt3 == LOW) {
    Serial.println("Button 3 pressed");
  }

  // Deteta a transição do botão 4: HIGH para LOW
  if (ultimoBotaoExt4 == HIGH && botaoAtualExt4 == LOW) {
    Serial.println("Button 4 pressed");
  }


  // Atualiza o estado anterior do botão 1
  ultimoBotaoExt1 = botaoAtualExt1;

  // Atualiza o estado anterior do botão 2
  ultimoBotaoExt2 = botaoAtualExt2;

  // Atualiza o estado anterior do botão 3
  ultimoBotaoExt3 = botaoAtualExt3;

  // Atualiza o estado anterior do botão 4
  ultimoBotaoExt4 = botaoAtualExt4;


  // Pequeno atraso para debounce básico
  delay(50);
}
