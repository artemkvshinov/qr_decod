#include <Adafruit_NeoPixel.h>

int pov1 = 50;             // настройка скорости поворотника
int pov2 = 80;             //настройка скорости поворотника (для вашей машины)
int y = 5;                  // яркость нулевой белый

const int PinNP = 19;       //№ пина для подключения WS2812B

const int NeoPix = 12;     //Количество светодиодов в ленте

const int butPinLeft = 12;  //№ пина для подключения Левого поворота
const int butPinRight = 13;  //№ пина для подключения Правого поворота

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NeoPix, PinNP, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(butPinLeft, INPUT);
  pinMode(butPinRight, INPUT);

  digitalWrite(butPinLeft, LOW);
  digitalWrite(butPinRight, LOW);

  strip.begin();

  strip.show();
  strip.setBrightness(150);
  // поочередно включаем красный цвет
  for (int i = -1; i < NeoPix / 3; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.show();
    delay(50);
  }
  for (int i = NeoPix / 3; i > -1; i--) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
    delay(50);
  }
  // поочередно включаем зеленый цвет
  for (int i = NeoPix / 3; i < NeoPix / 3 * 2; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
    strip.show();
    delay(50);
  }
  for (int i = NeoPix / 3 * 2; i > NeoPix / 3 - 1; i--) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
    delay(50);
  }
  // поочередно включаем синий цвет
  for (int i = NeoPix / 3 * 2; i < NeoPix; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
    strip.show();
    delay(50);
  }
  for (int i = NeoPix; i > NeoPix / 3 * 2 - 1; i--) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
    delay(50);
  }
  strip.clear();

}
void loop() {

  //  Aварийное мигание обоих поворотов
  if (digitalRead(butPinLeft) == HIGH && digitalRead(butPinRight) == HIGH) {
    for (int il = 0; il < NeoPix;  il++ ) {
      strip.setPixelColor(NeoPix / 2 - il, strip.Color(63, 31, 0));
      strip.setPixelColor(il + NeoPix / 2, strip.Color(63, 31, 0));
      strip.show();
      delay(pov1);
    }
    for (int il = 0; il < NeoPix; il++ ) {
      strip.setPixelColor(NeoPix / 2 - il, strip.Color(0, 0, 0));
      strip.setPixelColor(il + NeoPix / 2, strip.Color(0, 0, 0));
      strip.show();
      delay(pov1);
    }
    delay(pov2);
  }

  else {
    // ДХО
    if (digitalRead(butPinLeft) == LOW && digitalRead(butPinRight) == LOW) {
      for (int i = 0; i < NeoPix; i++ ) strip.setPixelColor(i, strip.Color(y, y, y));
      delay(pov2 - 20);
      strip.show();

    }

  }

  // ЛЕВЫЙ ПОВОРОТ
  if (digitalRead(butPinLeft) == HIGH && digitalRead(butPinRight) == LOW) {
    for (int il = 0; il < NeoPix; il++ ) {
      strip.setPixelColor(il + NeoPix / 2, strip.Color(60, 10, 0));
      strip.show();
      delay(pov1);
    }
    for (int il = 0; il < NeoPix; il++ ) {
      strip.setPixelColor(il + NeoPix / 2, strip.Color(0, 0, 0));
      strip.show();
      delay(pov1);
    }
    delay(pov2);
  }

  else {
    // ДХО
    if (digitalRead(butPinLeft) == LOW && digitalRead(butPinRight) == LOW) {
      for (int i = 0; i < NeoPix; i++ ) strip.setPixelColor(i, strip.Color(y, y, y));
      delay(pov2 - 20);
      strip.show();

    }

  }

  // ПРАВЫЙ ПОВОРОТ
  if (digitalRead(butPinLeft) == LOW && digitalRead(butPinRight) == HIGH) {
    for (int il = 1; il < NeoPix; il++ ) {
      strip.setPixelColor(NeoPix / 2 - il, strip.Color(60, 10, 0));
      strip.show();
      delay(pov1);
    }
    for (int il = 1; il < NeoPix; il++ ) {
      strip.setPixelColor(NeoPix / 2 - il, strip.Color(0, 0, 0));
      strip.show();
      delay(pov1);
    }
    delay(pov2);
  }
  else {
    // ДХО
    if (digitalRead(butPinLeft) == LOW && digitalRead(butPinRight) == LOW) {
      for (int i = 0; i < NeoPix; i++ ) strip.setPixelColor(i, strip.Color(y, y, y));
      delay(pov2 - 20);
      strip.show();

    }

  }
  delay(15);
}
//Shaitan4ik
