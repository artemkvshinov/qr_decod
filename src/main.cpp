#include "include.h"

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
</head>
<body>
  <div h1>Hello World<h1></div>
</body>
</html>)rawliteral";


#define LED_ON_BORD 4

char* password = "admin";

char* WiFi_ssid = "MKASS";
char* WiFi_password = "*MzVcT1#";
char* Soft_AP_ssid = "KATYA";
char* Soft_AP_password = "12345678";
IPAddress local_IP(192, 168, 4, 100);
// Укажите IP-адрес шлюза
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // опционально
IPAddress secondaryDNS(8, 8, 4, 4); // опционально


WebServer server(80);
const char* www_username = "admin";


void setup() {
  Serial.begin(115200); //открываем UART

  pinMode(LED_ON_BORD,OUTPUT);


  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {  // Настраивает статический IP-адрес
    Serial.println("STA Failed To Configure");                              
  }
  WiFi.mode(WIFI_STA);                                                      
  WiFi.begin(WiFi_ssid, WiFi_password);                                     // Пытаемя подключится к точки достпупа 
  Serial.println("WiFi Connecting");
  for (uint8_t i;WiFi.waitForConnectResult() != WL_CONNECTED;i++) {         // щетчик от 0 до 10сек кода настанет 10 сек мы поднимаем свою точку доступа
    Serial.print(".");
    digitalWrite(LED_ON_BORD,!digitalRead(LED_ON_BORD));
    if(i == 40){
      Serial.println("WiFi Connect Failed!\nEnabling The Access Point!");
      WiFi.softAP(Soft_AP_ssid, Soft_AP_password);                          // поднимаем свою точку доступа
      break;
    }
    delay(250);
  }
  digitalWrite(LED_ON_BORD,LOW);

  /*
  ArduinoOTA.begin();

  server.on("/", []() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
    server.send(200, "text/html", index_html);
  });
  server.begin();

  Serial.print("IP :");
  Serial.println(WiFi.localIP());
  */
}

void loop() {
  /*
  ArduinoOTA.handle();
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
  */
  if(Serial.available()){
    
  }
}