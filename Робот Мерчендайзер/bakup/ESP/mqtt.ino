#include <WiFi.h>
#include <PubSubClient.h>

#define mqttLed 5


#define ssid "CMIT" // Имя роутера
const char *pass = "CMIT2022"; // Пароль роутера
const char *mqtt_server = "m13.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 10926; // Порт для подключения к серверу MQTT
const char *mqtt_user = "vwaxgwrh"; // Логи для подключения к серверу MQTT
const char *mqtt_pass = "oAYBGpDO4Q6M"; // Пароль для подключения к серверу MQTT


WiFiClient wclient;
PubSubClient client(mqtt_server, mqtt_port, wclient);


void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(Right_UP, OUTPUT);
  pinMode(Right_Down, OUTPUT);
  pinMode(Left_UP, OUTPUT);
  pinMode(Left_Down, OUTPUT);
  pinMode(mqttLed, OUTPUT);
  digitalWrite(mqttLed, LOW);

  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(Right_PWM, ledChannel);
  ledcAttachPin(Left_PWM, ledChannel);
  
}


// Функция получения данных от сервера
void receivedCallback(char* topic, byte* payload, unsigned int length) {
  String msg = String((char*)payload);
  msg = msg.substring(0, length);
  Serial.println(msg);

  //функция скорости
  if (msg[0] == 's') {
    pwm = msg.substring(1, length).toInt();
       ledcWrite(ledChannel, pwm);
  }


  if (msg == "press_up")  forward();          //если кнопка вперёд нажата
  if (msg == "unpress_up") stopall();         //если кнопка вперёд отжата
  if (msg == "press_down") backward();        //если кнопка назад нажата
  if (msg == "unpress_down") stopall();       //если кнопка назад отжата
  if (msg == "press_right") right();          //если кнопка в право нажата
  if (msg == "unpress_right") stopall();      //если кнопка в право отжата
  if (msg == "press_left") left();            //если кнопка в лево нажата
  if (msg == "unpress_left") stopall();       //если кнопка в лево отжата
}


void loop() {
  // подключаемся к wi-fi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED) return;
    Serial.println("WiFi connected");
  }

  // подключаемся к MQTT серверу
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.print("Connecting to MQTT server ");
      Serial.print(mqtt_server);
      Serial.println("...");
      if (client.connect("esp32", mqtt_user, mqtt_pass)) {
        Serial.println("Connected to MQTT server ");
        digitalWrite(mqttLed, HIGH);
        client.setCallback(receivedCallback);
        // подписываемся под топики
        client.subscribe("testtopic");

      } else {
        Serial.println("Could not connect to MQTT server");
      }
    }

    if (client.connected()) {
      client.loop();

    }

  }
}
