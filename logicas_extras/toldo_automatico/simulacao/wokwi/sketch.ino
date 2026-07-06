#include <Servo.h>
#define sensorchuva 2
const int pinoServo = 9;
int pos = 0;
Servo meuServo;

void setup() {
  meuServo.attach(pinoServo);
  pinMode(sensorchuva, INPUT);
  meuServo.write(0);
}

void loop() {
  int estadoclimatico = digitalRead(sensorchuva);
  if (estadoclimatico == HIGH) {

    for (pos = 0; pos <= 90; pos ++) {
      meuServo.write(pos);
      delay(50);
    }
    delay(8000);
    for(pos=90;pos>=0;pos--){
      meuServo.write(pos);
      delay(50);
    }
  }
}
