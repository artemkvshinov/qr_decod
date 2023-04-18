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
// опросчик и парсер сериал
void serialTick() {
  if (Serial.available() > 0) {           // проверка данных на вход
    String buf = Serial.readString();     // читаем как string
    // механизм такой: командой startsWith сравниваем начало строки
    // если совпадает - делаем какие то действия
    // для приёма значений используется такой механизм:
    // строка обрезается командой substring до длины команды .substring(команда.length())
    // оставшееся число преобразуется в число командой .toInt()


    if (buf.startsWith(reboot)) {
      Serial.println("Reboot After 1 Second");
      delay(1000);
      ESP.restart();
    }

    else if (buf.startsWith(get_WiFi_ssid)) {
      Serial.print("SSID is ");
      Serial.println(WiFi_ssid);
    }

    else if (buf.startsWith(set_WiFi_ssid)) {
      buf.substring(set_WiFi_ssid.length()).toCharArray(WiFi_ssid, 32);
      Serial.print("SSID set to ");
      Serial.println(WiFi_ssid);
    }

    else if (buf.startsWith(get_WiFi_passwd)) {
      Serial.print("Password is ");
      Serial.println(get_WiFi_passwd);
    }

    else if (buf.startsWith(set_WiFi_passwd)) {
      buf.substring(set_WiFi_passwd.length()).toCharArray(WiFi_password, 32);
      Serial.print("Password set to ");
      Serial.println(WiFi_password);
    }

    else if (buf.startsWith(get_softAP_ssid)) {
      Serial.print("SSID SoftAP is ");
      Serial.println(Soft_AP_ssid);
    }

    else if (buf.startsWith(set_softAP_ssid)) {
      buf.substring(set_softAP_ssid.length()).toCharArray(Soft_AP_ssid, 32);
      Serial.print("SSID SoftAP set to ");
      Serial.println(Soft_AP_ssid);
    }

    else if (buf.startsWith(get_softAP_passwd)) {
      Serial.print("Password SoftAP is ");
      Serial.println(Soft_AP_password);
    }
    else if (buf.startsWith(set_softAP_passwd)) {
      buf.substring(set_softAP_passwd.length()).toCharArray(Soft_AP_password, 32);
      Serial.print("Password SoftAPs set to ");
      Serial.println(Soft_AP_password);
    }

    else if (buf.startsWith(WiFi_Reset)) {
      Serial.println("WiFi Reset");
      WiFi_start();
    }

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

    else {
      Serial.print("Command : "); Serial.print(buf); Serial.println(" Not Found");
    }
  }
}
