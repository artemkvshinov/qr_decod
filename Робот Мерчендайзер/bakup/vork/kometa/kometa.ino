#include "GyverPWM.h"м
#define intAR 2             //сигнал движения против часовой стрелки правых моторчиков
#define intBR 3             //сигнал движения по часовой стрелки правых моторчиков
#define intAL 4             //сигнал движения против часовой стрелки левых моторчиков
#define intBL 5             //сигнал движения по часовой стрелки левых моторчиков

#define PWM1 10             //пин шим 

void setup() {
  Serial.begin(9600);       //открыаем последовательны порт на скорости 9600 бод
  pinMode(intAR, OUTPUT);   //пин D2 настраеваем как выход
  pinMode(intBR, OUTPUT);   //пин D3 настраеваем как выход
  pinMode(intAL, OUTPUT);   //пин D4 настраеваем как выход
  pinMode(intBL, OUTPUT);   //пин D5 настраеваем как выход
  pinMode(PWM1,  OUTPUT);   //пин D10 настраеваем как выход


  // Данные функции убирают один ШИМ выход у 8-ми битных таймеров, оставляя нам ШИМ пины D3, D5, D9 и D10 на ATmega328
  // запустить ШИМ на D9 с частотой 150'000 Гц, режим FAST_PWM
  PWM_frequency(PWM1, 180000, FAST_PWM);  // запустить ШИМ на D10 с частотой 150'000 Гц, режим FAST_PWM

  digitalWrite(intAR, LOW);
  digitalWrite(intBR, LOW);
  digitalWrite(intAL, LOW);
  digitalWrite(intBL, LOW);

}

char key;
uint8_t val;
void loop() {
  if (Serial.available() > 1) {
    key = Serial.read();
    val = Serial.parseInt();
  }
  switch (key) {
    case'u':
      digitalWrite(intAR, HIGH);
      digitalWrite(intBR, LOW);
      digitalWrite(intAL, HIGH);
      digitalWrite(intBL, LOW);
      Serial.println("up");
      break;
    case'd':
      digitalWrite(intAR, LOW);
      digitalWrite(intBR, HIGH);
      digitalWrite(intAL, LOW);
      digitalWrite(intBL, HIGH);
      Serial.println("down");
      break;
    case'r':
      digitalWrite(intAR, HIGH);
      digitalWrite(intBR, LOW);
      digitalWrite(intAL, LOW);
      digitalWrite(intBL, HIGH);
      Serial.println("rigih");
      break;
    case'l':
      digitalWrite(intAR, LOW);
      digitalWrite(intBR, HIGH);
      digitalWrite(intAL, HIGH);
      digitalWrite(intBL, LOW);
      Serial.println("left");
      break;
    case'p':
      digitalWrite(intAR, LOW);
      digitalWrite(intBR, LOW);
      digitalWrite(intAL, LOW);
      digitalWrite(intBL, LOW);
      Serial.println("break possiv");
      break;
    case'a':
      digitalWrite(intAR, HIGH);
      digitalWrite(intBR, HIGH);
      digitalWrite(intAL, HIGH);
      digitalWrite(intBL, HIGH);
      Serial.println("break active");
      break;
    case'b':
      analogWrite(PWM1, val);
      Serial.print("pwm is "); Serial.println(val);
      break;
  }
}
