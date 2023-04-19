// строки текстовых команд
String reboot = "Reboot";
String get_WiFi_ssid = "Get WiFi SSID";
String set_WiFi_ssid = "Set WiFi SSID ";
String get_WiFi_passwd = "Get WiFi Password";
String set_WiFi_passwd = "Set WiFi Password ";
String get_softAP_ssid = "Get SoftAP SSID";
String set_softAP_ssid = "Set SoftAP SSID ";
String get_softAP_passwd = "Get SoftAP Password";
String set_softAP_passwd = "Set SoftAP Password ";
String WiFi_Reset = "WiFi Reset";
String get_WiFI_conf = "Get WiFi Config";
String Scan_WiFi = "Scan WiFi";

// опросчик и парсер сериал
void serialTick() {
  if (Serial.available() > 0) {           // проверка данных на вход
    String buf = Serial.readString();     // читаем как string
    // механизм такой: командой startsWith сравниваем начало строки
    // если совпадает - делаем какие то действия
    // для приёма значений используется такой механизм:
    // строка обрезается командой substring до длины команды .substring(команда.length())

    //функция перезагрузки
    if (buf.startsWith(reboot)) {
      Serial.println("Reboot After 1 Second");
      delay(1000);
      ESP.restart();
    }

    //вывод SSID
    else if (buf.startsWith(get_WiFi_ssid)) {
      Serial.print("SSID is ");
      Serial.println(WiFi_ssid);
    }

    //установка SSID
    else if (buf.startsWith(set_WiFi_ssid)) {
      buf.substring(set_WiFi_ssid.length()).toCharArray(WiFi_ssid, 32);
      Serial.print("SSID set to ");
      Serial.println(WiFi_ssid);
#if AUTOMATIC_RESTART_WIFI_AFTER_WIFI_CONFIG
      WiFi_start();
#endif
    }

    //вывод пароля от WiFi
    else if (buf.startsWith(get_WiFi_passwd)) {
      Serial.print("Password is ");
      Serial.println(get_WiFi_passwd);
    }

    //установка пароля от WiFi
    else if (buf.startsWith(set_WiFi_passwd)) {
      buf.substring(set_WiFi_passwd.length()).toCharArray(WiFi_password, 32);
      Serial.print("Password set to ");
      Serial.println(WiFi_password);
#if AUTOMATIC_RESTART_WIFI_AFTER_WIFI_CONFIG
      WiFi_start();
#endif
    }

    //вывод SSID от точки доступа
    else if (buf.startsWith(get_softAP_ssid)) {
      Serial.print("SSID SoftAP is ");
      Serial.println(Soft_AP_ssid);
    }

    //установка SSID от точки доступа
    else if (buf.startsWith(set_softAP_ssid)) {
      buf.substring(set_softAP_ssid.length()).toCharArray(Soft_AP_ssid, 32);
      Serial.print("SSID SoftAP set to ");
      Serial.println(Soft_AP_ssid);
#if AUTOMATIC_RESTART_WIFI_AFTER_WIFI_CONFIG
      WiFi_start();
#endif
    }

    //вывод пароля от точки доступа
    else if (buf.startsWith(get_softAP_passwd)) {
      Serial.print("Password SoftAP is ");
      Serial.println(Soft_AP_password);
    }

    //установка пароля от точки доступа
    else if (buf.startsWith(set_softAP_passwd)) {
      buf.substring(set_softAP_passwd.length()).toCharArray(Soft_AP_password, 32);
      Serial.print("Password SoftAPs set to ");
      Serial.println(Soft_AP_password);
#if AUTOMATIC_RESTART_WIFI_AFTER_WIFI_CONFIG
      WiFi_start();
#endif
    }

    //перезапуск функции инициализации WiFi
    else if (buf.startsWith(WiFi_Reset)) {
      Serial.println("WiFi Reset");
      WiFi_start();
    }

    //вывод даных о WiFi
    else if (buf.startsWith(get_WiFI_conf)) {
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      Serial.print("Hostname: ");
      Serial.println(WiFi.getHostname());
      Serial.print("ESP Mac Address: ");
      Serial.println(WiFi.macAddress());
      Serial.print("Subnet Mask: ");
      Serial.println(WiFi.subnetMask());
      Serial.print("Gateway IP: ");
      Serial.println(WiFi.gatewayIP());
      Serial.print("DNS: ");
      Serial.println(WiFi.dnsIP());
    }

    //сканирование точек доступа
    else if (buf.startsWith(Scan_WiFi)) {
      WiFi.mode(WIFI_STA);
      WiFi.disconnect();
      delay(100);
      Serial.println("Scan start");

      // WiFi.scanNetworks will return the number of networks found.
      int n = WiFi.scanNetworks();
      Serial.println("Scan done");
      if (n == 0) {
        Serial.println("no networks found");
      } else {
        Serial.print(n);
        Serial.println(" networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          Serial.printf("%2d", i + 1);
          Serial.print(" | ");
          Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
          Serial.print(" | ");
          Serial.printf("%4d", WiFi.RSSI(i));
          Serial.print(" | ");
          Serial.printf("%2d", WiFi.channel(i));
          Serial.print(" | ");
          switch (WiFi.encryptionType(i))
          {
            case WIFI_AUTH_OPEN:
              Serial.print("open");
              break;
            case WIFI_AUTH_WEP:
              Serial.print("WEP");
              break;
            case WIFI_AUTH_WPA_PSK:
              Serial.print("WPA");
              break;
            case WIFI_AUTH_WPA2_PSK:
              Serial.print("WPA2");
              break;
            case WIFI_AUTH_WPA_WPA2_PSK:
              Serial.print("WPA+WPA2");
              break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
              Serial.print("WPA2-EAP");
              break;
            case WIFI_AUTH_WPA3_PSK:
              Serial.print("WPA3");
              break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
              Serial.print("WPA2+WPA3");
              break;
            case WIFI_AUTH_WAPI_PSK:
              Serial.print("WAPI");
              break;
            default:
              Serial.print("unknown");
          }
          Serial.println();
          delay(10);
        }
      }
      Serial.println("");

      // Delete the scan result to free memory for code below.
      WiFi.scanDelete();
#if AUTOMATIC_CONNECTION_AFTER_WIFI_SCANNING
      WiFi_start();
#endif

    }

    //если команда введена непраельно или такой команды нет
    else {
      Serial.print("Command : "); Serial.print(buf); Serial.println(" Not Found");
    }
  }
}
