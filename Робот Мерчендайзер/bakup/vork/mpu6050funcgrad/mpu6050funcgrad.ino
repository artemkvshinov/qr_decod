#include "MPU6050.h"
MPU6050 mpu;
void setup() {
  Wire.begin();
  Serial.begin(9600);
  mpu.initialize();     // запускаем датчик
}
void loop() {
  int16_t ax = mpu.getAccelerationX();  // ускорение по оси Х
  int16_t ay = mpu.getAccelerationY();  // ускорение по оси Y
  int16_t az = mpu.getAccelerationZ();  // ускорение по оси Z
  ax = ark(ax);

  Serial.print(ax);Serial.print("     ");Serial.print(ark(ay));Serial.print("     ");Serial.println(ark(az));
  delay(50);
}

int ark(int znach) {
  // стандартный диапазон: +-2g
  znach = constrain(znach, -16384, 16384);    // ограничиваем +-1g
  float angle = znach / 16384.0;           // переводим в +-1.0
  // и в угол через арксинус
  if (angle < 0) angle = 90 - degrees(acos(angle));
  else angle = degrees(acos(-angle)) - 90;
  return angle;
}
