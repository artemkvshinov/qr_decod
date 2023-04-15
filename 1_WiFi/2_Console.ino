// строки текстовых команд
String reboot = "Reboot";
String get_WiFi_ssid = "Get WiFi SSID ";
String set_WiFi_ssid = "Set WiFi SSID ";
String get_WiFi_passwd = "Get WiFi Password ";
String set_WiFi_passwd = "Set WiFi Password ";
String WiFi_Reset = "WiFi Reset";
String get_my_ip = "My IP";
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
      
      buf.substring(set_WiFi_ssid.length()).toCharArray(WiFi_ssid, 8);
      Serial.print("SSID set to ");
      Serial.println(WiFi_ssid);
    }
    else if (buf.startsWith(get_WiFi_passwd)) {
      Serial.print("Password is ");
      Serial.println(get_WiFi_passwd);
    }
    else if (buf.startsWith(set_WiFi_passwd)) {
      
      buf.substring(set_WiFi_passwd.length()).toCharArray(WiFi_password, 8);
      Serial.print("Password set to ");
      Serial.println(WiFi_password);
    }
    else if (buf.startsWith(WiFi_Reset)) {
      Serial.println("WiFi Reset");
      WiFi_start();
    }
    else if (buf.startsWith(get_my_ip)) {
      Serial.print("My IP is "); Serial.println(WiFi.localIP());
    }
    else {
      Serial.print("Comand : "); Serial.print(buf); Serial.println(" Not Found");
    }
  }
}
