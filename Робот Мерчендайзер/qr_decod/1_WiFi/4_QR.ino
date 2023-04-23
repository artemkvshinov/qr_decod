void setup() {
  Serial.begin(115200); //открываем UART

  pinMode(LED_ON_BORD, OUTPUT);
  WiFi_start();
}
void loop() {
  serialTick();
}
