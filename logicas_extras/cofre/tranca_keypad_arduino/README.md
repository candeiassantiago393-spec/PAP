# Tranca de Cofre com Keypad e Arduino

## Descrição

Este projeto consiste numa tranca de cofre controlada por Arduino, utilizando um teclado matricial 4x3, um servomotor, dois LEDs e um buzzer. O sistema permite abrir a tranca apenas quando é introduzida a senha correta.

O objetivo principal é simular um sistema simples de controlo de acesso, semelhante ao funcionamento de uma fechadura eletrónica.

## Funcionamento

O utilizador introduz uma senha através do keypad.

A tecla `#` serve para confirmar o código introduzido.

A tecla `*` serve para apagar/resetar o código introduzido.

Se o código estiver correto, o LED verde acende, o buzzer emite um sinal sonoro e o servomotor roda para abrir a tranca(90º).

Se o código estiver incorreto, o LED vermelho acende e o buzzer emite um som de erro(servo sem movimento).

## Tabela de Ligações

| Componente | Terminal / Pino do componente | Pino no Arduino | Observações |
|---|---|---|---|
| Servomotor | Sinal | D9 | Controla a abertura/fecho da tranca |
| Servomotor | VCC | 5V | Alimentação do servo |
| Servomotor | GND | GND | Massa comum |
| LED vermelho | Ânodo (+) | D11 | Usar resistência de 220 Ω ou 330 Ω |
| LED vermelho | Cátodo (-) | GND | Ligação à massa |
| LED verde | Ânodo (+) | D12 | Usar resistência de 220 Ω ou 330 Ω |
| LED verde | Cátodo (-) | GND | Ligação à massa |
| Buzzer | Positivo (+) | D13 | Emite som de aviso |
| Buzzer | Negativo (-) | GND | Ligação à massa |
| Keypad 4x3 | Linha 1 | D5 | Primeira linha do teclado |
| Keypad 4x3 | Linha 2 | D4 | Segunda linha do teclado |
| Keypad 4x3 | Linha 3 | D3 | Terceira linha do teclado |
| Keypad 4x3 | Linha 4 | D2 | Quarta linha do teclado |
| Keypad 4x3 | Coluna 1 | D8 | Primeira coluna do teclado |
| Keypad 4x3 | Coluna 2 | D7 | Segunda coluna do teclado |
| Keypad 4x3 | Coluna 3 | D6 | Terceira coluna do teclado |

## Código de acesso

A senha definida no programa é `1904`.

Depois de introduzir a senha, o utilizador deve premir `#` para confirmar.

Exemplo:

```txt
1904#





