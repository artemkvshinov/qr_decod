//настройки
#define ENABLING_THE_ACCESS_POINT 1 //включть точку доступа если подключение к WiFi непроизошло
/*
 * если ENABLING_THE_ACCESS_POINT = 0 то
 *    при поытке подключения к WiFi Esp32 зависает в бесконечном пока не подключится к WiFi 
 * если ENABLING_THE_ACCESS_POINT = 1 то
 *    при попытки подключения к WiFi запускается таймер
 *    если через 10 секунд подключение к WiFi не устоновлено, включается точа доступа
 */
 
#define AUTOMATIC_CONNECTION_AFTER_WIFI_SCANNING 1
/*
 * если AUTOMATIC_CONNECTION_AFTER_WIFI_SCANNING = 0 то
 *    после сканирования WiFi надо заново ввести команду "WiFi Reset" чтобы перезапустить WiFi
 * если AUTOMATIC_CONNECTION_AFTER_WIFI_SCANNING = 1 то
 *    после сканирования WiFi автоматический вызывается функция WiFi_start() которая перезапускает WiFi
 */

#define AUTOMATIC_RESTART_WIFI_AFTER_WIFI_CONFIG 0
/*
 * если AUTOMATIC_RESTART_WIFI_AFTER_WIFI_CONFIG = 0 то
 *    после ввода "Set WiFi SSID " "Set WiFi Password " "Set SoftAP SSID " "Set SoftAP Password " надо в ручную ввести "WiFi Reset" чтобы измененя вступили в силу
 * если AUTOMATIC_RESTART_WIFI_AFTER_WIFI_CONFIG = 1 то
 *    при вводе "Set WiFi SSID " "Set WiFi Password " "Set SoftAP SSID " "Set SoftAP Password " будет вызываться функция WiFi_start()
 */
