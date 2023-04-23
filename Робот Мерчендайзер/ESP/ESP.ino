#define Right_PWM 15
#define Right_UP 0
#define Right_Down 2
#define Left_PWM 4
#define Left_UP 17
#define Left_Down 16
#define freq 4500       //чистота шим в Гц 
#define ledChannel 0    //канал щим(от 0 до 15)
#define resolution 8    //разрешение шим
byte pwm = 125;


void backward() {              //назад
  digitalWrite(Right_UP, LOW);
  digitalWrite(Right_Down, HIGH);
  digitalWrite(Left_UP, LOW);
  digitalWrite(Left_Down, HIGH);
}

void forward() {                //вперёд
  digitalWrite(Right_UP, HIGH);
  digitalWrite(Right_Down, LOW);
  digitalWrite(Left_UP, HIGH);
  digitalWrite(Left_Down, LOW);
}

void right() {                //право
  digitalWrite(Right_UP, HIGH);
  digitalWrite(Right_Down, LOW);
  digitalWrite(Left_UP, LOW);
  digitalWrite(Left_Down, HIGH);
}

void left() {                //лево
  digitalWrite(Right_UP, LOW);
  digitalWrite(Right_Down, HIGH);
  digitalWrite(Left_UP, HIGH);
  digitalWrite(Left_Down, LOW);
}

void stopall() {                //стоп
  digitalWrite(Right_UP, LOW);
  digitalWrite(Right_Down, LOW);
  digitalWrite(Left_UP, LOW);
  digitalWrite(Left_Down, LOW);
}
