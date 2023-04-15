#include <WiFi.h>
#include <ESPmDNS.h>

#define LED_ON_BORD 4

char* WiFi_ssid = "MKASS";
char* WiFi_password = "*MzVcT1#";
char* Soft_AP_ssid = "KATYA";
char* Soft_AP_password = "12345678";
IPAddress local_IP(192, 168, 43, 100);
// Укажите IP-адрес шлюза
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // опционально
IPAddress secondaryDNS(8, 8, 4, 4); // опционально





void WiFi_start(){
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {  // Настраивает статический IP-адрес
    Serial.println("STA Failed To Configure");
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi_ssid, WiFi_password);                                     // Пытаемя подключится к точки достпупа
  Serial.println("WiFi Connecting");
  for (uint8_t i; WiFi.waitForConnectResult() != WL_CONNECTED; i++) {       // щетчик от 0 до 10сек кода настанет 10 сек мы поднимаем свою точку доступа
    Serial.print(".");
    digitalWrite(LED_ON_BORD, !digitalRead(LED_ON_BORD));
    if (i == 100) {
      Serial.println("\nWiFi Connect Failed!\nEnabling The Access Point!");
      WiFi.mode(WIFI_AP);
      WiFi.softAP(Soft_AP_ssid, Soft_AP_password);                          // поднимаем свою точку доступа
      break;
    }
    delay(100);
  }
  digitalWrite(LED_ON_BORD, LOW);
}
