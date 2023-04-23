#include <WebServer.h>
#include <WiFi.h>
//#include <ESPAsyncWebServer.h>
#include <esp32cam.h>
 
const char* WIFI_SSID = "MKASS";
const char* WIFI_PASS = "*MzVcT1#";

WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <style>
    .image {
      width: auto ;
      max-width: 100% ;
      height: auto ; 
    }
  </style>
</head>
<body>
  <center>
    <h1>Hello World<h1>
    <img src="/Mjpg" id="reloader" onLoad="setTimeout( () => { document.getElementById('reloader').src='/Mjpg' + '?' + new Date().getMilliseconds() } ,150)" />
    
  </center>
</body>
<script>
  
</script>
</html>)rawliteral";
 

static auto hiRes = esp32cam::Resolution::find(800, 600);

 
static void serveMjpeg()
{
  Serial.println("MJPEG streaming begin");
  WiFiClient client = server.client();
  auto startTime = millis();
  int nFrames = esp32cam::Camera.streamMjpeg(server.send(200,"image/mjpg"));
  auto duration = millis() - startTime;
  Serial.printf("MJPEG streaming end: %dfrm %0.2ffps\n", nFrames, 1000.0 * nFrames / duration);
}

void serveManagement(){
  server.send(200, "text/html", index_html);
  WiFiClient client = server.client();;
}

 
void  setup(){
  Serial.begin(115200);
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
 
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.println("\t/Mjpg");
  Serial.println("\t/management");
 

  
  server.on("/stop",HTTP_GET, [](){
    server.send(200,"text/html","Done");
    Serial.println("stop");
  });
  server.on("/Mjpg", serveMjpeg);
  server.on("/management", serveManagement);
  server.begin();
}
 
void loop()
{
  server.handleClient();
}
