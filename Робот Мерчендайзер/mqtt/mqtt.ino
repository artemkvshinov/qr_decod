#include <WiFi.h>
#include <PubSubClient.h>

#define mqttLed 16 
#define RightUP 4
#define RightDown 0
#define LeftUP 2
#define LeftDown 15
#define BUFFER_SIZE 100

#define Dac1 25

const char *ssid = "CMIT"; // Имя роутера
const char *pass = "CMIT2022"; // Пароль роутера
const char *mqtt_server = "m13.cloudmqtt.com"; // Имя сервера MQTT
const int mqtt_port = 10926; // Порт для подключения к серверу MQTT
const char *mqtt_user = "vwaxgwrh"; // Логи для подключения к серверу MQTT
const char *mqtt_pass = "oAYBGpDO4Q6M"; // Пароль для подключения к серверу MQTT

const int led = 5; // диод на плате

int tm = 300;
float temp = 0;

byte pwm=0;

WiFiClient wclient; 
PubSubClient client(mqtt_server,mqtt_port,wclient);

void backward(){               //назад  
  digitalWrite(RightUP,HIGH);
  digitalWrite(RightDown,LOW);
  digitalWrite(LeftUP,HIGH);
  digitalWrite(LeftDown,LOW);
}

void forward(){                 //вперёд
  digitalWrite(RightUP,LOW);
  digitalWrite(RightDown,HIGH);
  digitalWrite(LeftUP,LOW);
  digitalWrite(LeftDown,HIGH);
}

void left(){                 //право
  digitalWrite(RightUP,HIGH);
  digitalWrite(RightDown,LOW);
  digitalWrite(LeftUP,LOW);
  digitalWrite(LeftDown,HIGH);
}

void right(){                 //лево
  digitalWrite(RightUP,LOW);
  digitalWrite(RightDown,HIGH);
  digitalWrite(LeftUP,HIGH);
  digitalWrite(LeftDown,LOW);
}

void stopall(){                 //стоп
  digitalWrite(RightUP,LOW);
  digitalWrite(RightDown,LOW);
  digitalWrite(LeftUP,LOW);
  digitalWrite(LeftDown,LOW);
}

void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(led, OUTPUT);
  
  pinMode(RightUP, OUTPUT);
  pinMode(RightDown, OUTPUT);
  pinMode(LeftUP, OUTPUT);
  pinMode(LeftDown, OUTPUT);
  pinMode(mqttLed, OUTPUT);
  digitalWrite(mqttLed,LOW);
}


//client.publish("test/1", time);


// Функция получения данных от сервера
void receivedCallback(char* topic, byte* payload, unsigned int length) 
{
 /* String payload = pub.payload_string();
  String topic = pub.topic();
  
  Serial.print(pub.topic()); // выводим в сериал порт название топика
  Serial.print(" => ");
  Serial.println(payload); // выводим в сериал порт значение полученных данных
*/
   String msg=String((char*)payload);
   msg=msg.substring(0,length);
   Serial.println(msg);
   if (msg[0]=='s'){
    pwm=msg.substring(1,length).toInt();
    dacWrite(Dac1,pwm);
    Serial.println(pwm);
   }
  if(msg == "press_up")
  {
     forward();
  }
  if(msg == "unpress_up")
  {
    stopall();
  }

  if(msg == "press_down")
  {
     backward();
  }
  if(msg == "unpress_down")
  {
    stopall();
  }

  if(msg == "press_right")
  {
     right();
  }
  if(msg == "unpress_right")
  {
    stopall();
  }

  if(msg == "press_left")
  {
     left();
  }
  if(msg == "unpress_left")
  {
    stopall();
  }

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
      if (client.connect("esp32",mqtt_user,mqtt_pass)) {
        Serial.println("Connected to MQTT server ");
        digitalWrite(mqttLed,HIGH);
        client.setCallback(receivedCallback);
        // подписываемся под топики
        client.subscribe("testtopic");
        
      } else {
        Serial.println("Could not connect to MQTT server"); 
      }
    }
    
    if (client.connected()){
      client.loop();
      
    }
  
  }
}
