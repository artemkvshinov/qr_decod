
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// In this project, I recommend using a sufficient power supply for the ESP32 Cam. Because in this project in addition to the camera, the LEDs on the ESP32 Cam board are also lit, //
// added to light the external LEDs, namely the green and blue LEDs. You can find more detailed information about the power consumption of the ESP32 Cam on the internet.           //
// I say this because initially there were many errors in this project, such as the LED on the ESP32 Cam not working properly, the ESP32 Cam restarting continuously,               //
// the camera could not display on web pages and so on. After I double-checked, it turned out that it was due to a lack of power supply.                                            //
// So in this project I used a 5V 2A adapter power supply. Using that adapter, the project went well.                                                                               //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I realized that the code in this project could have been simplified or improved. But because of my limited knowledge of arduino code, so I can't do it yet.                      //
// I'm also still learning.                                                                                                                                                         //
// So if you can simplify or improve the Arduino code of this project, please let me know, so I can try it and share it.                                                            //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* ======================================== Including the libraries. */
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "quirc.h"
#include <WiFi.h>
#include "esp_http_server.h"
/* ======================================== */

/* ======================================== Select camera model */
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
//#define CAMERA_MODEL_M5STACK_WITHOUT_PSRAM
#define CAMERA_MODEL_AI_THINKER
/* ======================================== */


/* ======================================== GPIO of camera models */
#if defined(CAMERA_MODEL_WROVER_KIT)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    21
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27

#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      19
#define Y4_GPIO_NUM      18
#define Y3_GPIO_NUM       5
#define Y2_GPIO_NUM       4
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

#elif defined(CAMERA_MODEL_ESP_EYE)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    4
#define SIOD_GPIO_NUM    18
#define SIOC_GPIO_NUM    23

#define Y9_GPIO_NUM      36
#define Y8_GPIO_NUM      37
#define Y7_GPIO_NUM      38
#define Y6_GPIO_NUM      39
#define Y5_GPIO_NUM      35
#define Y4_GPIO_NUM      14
#define Y3_GPIO_NUM      13
#define Y2_GPIO_NUM      34
#define VSYNC_GPIO_NUM   5
#define HREF_GPIO_NUM    27
#define PCLK_GPIO_NUM    25

#elif defined(CAMERA_MODEL_M5STACK_PSRAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       32
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_M5STACK_WITHOUT_PSRAM)
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    15
#define XCLK_GPIO_NUM     27
#define SIOD_GPIO_NUM     25
#define SIOC_GPIO_NUM     23

#define Y9_GPIO_NUM       19
#define Y8_GPIO_NUM       36
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       39
#define Y5_GPIO_NUM        5
#define Y4_GPIO_NUM       34
#define Y3_GPIO_NUM       35
#define Y2_GPIO_NUM       17
#define VSYNC_GPIO_NUM    22
#define HREF_GPIO_NUM     26
#define PCLK_GPIO_NUM     21

#elif defined(CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22
#else
#error "Camera model not selected"
#endif
/* ======================================== */

//user GPIO
#define LED_OnBoard 4
#define PWM_MOTORS_PIN 1600

#define MOTOR_1_PIN_1    15
#define MOTOR_1_PIN_2    14
#define MOTOR_2_PIN_1    12
#define MOTOR_2_PIN_2    13

// creating a task handle
TaskHandle_t QRCodeReader_Task;

/* ======================================== Variables declaration */
struct QRCodeData
{
  bool valid;
  int dataType;
  uint8_t payload[1024];
  int payloadLen;
};

struct quirc *q = NULL;
uint8_t *image = NULL;
struct quirc_code code;
struct quirc_data data;
quirc_decode_error_t err;
struct QRCodeData qrCodeData;
String QRCodeResult = "";
String QRCodeResultSend = "";
/* ======================================== */

/* ======================================== */
bool ws_run = false;
int wsLive_val = 0;
int last_wsLive_val;
byte get_wsLive_interval = 0;
bool get_wsLive_val = true;
/* ======================================== */

/* ======================================== Variables for millis() */
unsigned long previousMillis = 0;
const long interval = 1000;
/* ======================================== */

/* ======================================== Replace with your network credentials */
const char* ssid = "CMIT";
const char* password = "CMIT2022";
/* ======================================== */

/* ======================================== */
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";
/* ======================================== */

/* ======================================== Empty handle to esp_http_server */
httpd_handle_t index_httpd = NULL;
httpd_handle_t stream_httpd = NULL;
/* ======================================== */


/* ======================================== HTML code for index / main page */
static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<html>

  <head>
  
    <title>ESP32-CAM QR Code Reader Stream Web Server</title>
    
    <meta name="viewport" content="width=device-width, initial-scale=1">
    
    <style>
    
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 10px;}

      /* ----------------------------------- Slider */
      .slidecontainer {
        width: 100%;
      }

      .slider {
        -webkit-appearance: none;
        width: 50%;
        height: 10px;
        border-radius: 5px;
        background: #d3d3d3;
        outline: none;
        opacity: 0.7;
        -webkit-transition: .2s;
        transition: opacity .2s;
      }

      .slider:hover {
        opacity: 1;
      }

      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #04AA6D;
        cursor: pointer;
      }

      .slider::-moz-range-thumb {
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #04AA6D;
        cursor: pointer;
      }
      .slider2::-moz-range-thumb {
        width: 20px;
        height: 20px;
        border-radius: 50%;
        background: #04AA6D;
        cursor: pointer;
      }

      
      /* ----------------------------------- */

      /* ----------------------------------- Stream Viewer */
      img {
        width: auto ;
        max-width: 100% ;
        height: auto ; 
      }
      /* ----------------------------------- */
      
    </style>
    
  </head>
  
  <body>
    <h1>ESP32-CAM Robot</h1>
    <h3>ESP32-CAM QR Code Reader Stream Web Server</h3>
    
    <img src="" id="vdstream">
    
    <br><br>
    
    <div class="slidecontainer">
      <span style="font-size:15;">LED Flash &nbsp;</span>
      <input type="range" min="0" max="20" value="0" class="slider" id="mySlider">
    </div>
    <h1> </h1>
    <div class="slidecontainer">
      <span style="font-size:15;">PWM Motors &nbsp;</span>
      <input type="range" min="0" max="255" value="0" class="slider" id="Slider_moto">
    </div>
    
    
    
    <br>

    <p>QR Code Scan Result :</p>
    <div style="padding: 5px; border: 3px solid #075264; text-align: center; width: 70%; margin: auto; color:#0A758F;" id="showqrcodeval"></div>
    
    <br>

    <button type="button" onclick="CopyQRCodeRslt()">Copy Result</button>
    <button type="button" onclick="send_btn_cmd('clr')">Clear Result</button>\
      <center>
        <table>
          <tr>
            <td colspan="3" align="center">
              <button class="button" 
                onmousedown="toggleCheckbox('forward');" 
                ontouchstart="toggleCheckbox('forward');" 
                onmouseup="toggleCheckbox('stop');" 
                ontouchend="toggleCheckbox('stop');">Forward
              </button>
            </td>
          </tr>
          <tr>
            <td align="center">
              <button class="button" 
                onmousedown="toggleCheckbox('left');" 
                ontouchstart="toggleCheckbox('left');" 
                onmouseup="toggleCheckbox('stop');" 
                ontouchend="toggleCheckbox('stop');">Left
              </button>
            </td>
            <td align="center">
              <button class="button" 
                onmousedown="toggleCheckbox('stop');" 
                ontouchstart="toggleCheckbox('stop');">Stop
              </button>
            </td>
            <td align="center">
              <button class="button" 
                onmousedown="toggleCheckbox('right');" 
                ontouchstart="toggleCheckbox('right');" 
                onmouseup="toggleCheckbox('stop');" 
                ontouchend="toggleCheckbox('stop');">Right
              </button>
            </td>
          </tr>
            <tr>
              <td colspan="3" align="center">
                <button class="button" 
                  onmousedown="toggleCheckbox('backward');" 
                  ontouchstart="toggleCheckbox('backward');" 
                  onmouseup="toggleCheckbox('stop');" 
                  ontouchend="toggleCheckbox('stop');">Backward
                </button>
              </td>
            </tr>                   
        </table>
      </center>
    
    <div p>apples :</p><style="padding: 5px; border: 3px solid #075264; text-align: center; width: 70%; margin: auto; color:#0A758F;" id="num_yablok"></div>
    
    <script>
    function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/action?go=" + x, true);
     xhr.send();
   }
      /* ----------------------------------- Calls the video stream link and displays it */ 
      window.onload = document.getElementById("vdstream").src = window.location.href.slice(0, -1) + ":81/stream";
      /* ----------------------------------- */
      
      var slider_pwm_led = document.getElementById("mySlider");
      var slider_pwm_motors = document.getElementById("Slider_moto");
      
      
      /* ----------------------------------- Variable declaration and timer to display QR Code reading results. */
      var myTmr;
      let qrcodeval = "...";
      start_timer();
      /* ----------------------------------- */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Update the current slider value (each time you drag the slider handle) */
      slider_pwm_led.oninput = function() {
        let slider_pwm_val = "S," + slider_pwm_led.value;
        send_cmd(slider_pwm_val);
      }
      slider_pwm_motors.oninput = function() {
        let slider_pwm_val = "M," + slider_pwm_motors.value;
        send_cmd(slider_pwm_val);
      }
      

      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function for sending commands */
      function send_cmd(cmds) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/action?go=" + cmds, true);
        xhr.send();
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Start and stop the timer */
      function start_timer() {
        myTmr = setInterval(myTimer, 500)
      }
      
      function stop_timer() {
        clearInterval(myTmr)
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Timer to get QR Code reading result and display it. */
      function myTimer() {
        getQRCodeVal();
        textQRCodeVal();
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to display the results of reading the QR Code. */
      function textQRCodeVal() {
        document.getElementById("showqrcodeval").innerHTML = qrcodeval;
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */
      
      
      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to send commands to the ESP32 Cam whenever the button is clicked. */
      function send_btn_cmd(cmds) {
        let btn_cmd = "B," + cmds;
        send_cmd(btn_cmd);
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to copy QR Code reading result to clipboard. */
      // Source: https://techoverflow.net/2018/03/30/copying-strings-to-the-clipboard-using-pure-javascript/
      function CopyQRCodeRslt() {
        // Create new element
        var el = document.createElement('textarea');
        // Set value (string to be copied)
        el.value = qrcodeval;
        // Set non-editable to avoid focus and move outside of view
        el.setAttribute('readonly', '');
        el.style = {position: 'absolute', left: '-9999px'};
        document.body.appendChild(el);
        // Select text inside element
        el.select();
        // Copy text to clipboard
        document.execCommand('copy');
        // Remove temporary element
        document.body.removeChild(el);
        /* Alert the copied text */
        alert("The result of reading the QR Code has been copied.");
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */

      /* :::::::::::::::::::::::::::::::::::::::::::::::: Function to get QR Code reading results. */
      function getQRCodeVal() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            qrcodeval = this.responseText;
          }
        };
        xhttp.open("GET", "/getqrcodeval", true);
        xhttp.send();
      }
      
      function get_num_yablok() {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            qrcodeval = this.responseText;
          }
        };
        xhttp.open("GET", "/getqrcodeval", true);
        xhttp.send();
      }
      /* :::::::::::::::::::::::::::::::::::::::::::::::: */
    </script>
  
  </body>
  
</html>
)rawliteral";
/* ======================================== */

/* ________________________________________________________________________________ Index handler function to be called during GET or uri request */
static esp_err_t index_handler(httpd_req_t *req){
  httpd_resp_set_type(req, "text/html");
  return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ stream handler function to be called during GET or uri request. */
static esp_err_t stream_handler(httpd_req_t *req){
  ws_run = true;
  vTaskDelete(QRCodeReader_Task);
  Serial.print("stream_handler running on core ");
  Serial.println(xPortGetCoreID());

  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char * part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if(res != ESP_OK){
    return res;
  }

  /* ---------------------------------------- Loop to show streaming video from ESP32 Cam camera and read QR Code. */
  while(true){
    ws_run = true;
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed (stream_handler)");
      res = ESP_FAIL;
    } else {
      q = quirc_new();
      if (q == NULL){
        Serial.print("can't create quirc object\r\n");  
        continue;
      }
      
      quirc_resize(q, fb->width, fb->height);
      image = quirc_begin(q, NULL, NULL);
      memcpy(image, fb->buf, fb->len);
      quirc_end(q);
      
      int count = quirc_count(q);
      if (count > 0) {
        quirc_extract(q, 0, &code);
        err = quirc_decode(&code, &data);
    
        if (err){
          QRCodeResult = "Decoding FAILED";
          Serial.println(QRCodeResult);
        } else {
          Serial.printf("Decoding successful:\n");
          dumpData(&data);
        } 
        Serial.println();
      } 
      
      image = NULL;  
      quirc_destroy(q);
      
      if(fb->width > 200){
        if(fb->format != PIXFORMAT_JPEG){
          bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
          esp_camera_fb_return(fb);
          fb = NULL;
          if(!jpeg_converted){
            Serial.println("JPEG compression failed");
            res = ESP_FAIL;
          }
        } else {
          _jpg_buf_len = fb->len;
          _jpg_buf = fb->buf;
        }
      }
    }
    if(res == ESP_OK){
      size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if(res == ESP_OK){
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    if(fb){
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if(_jpg_buf){
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    if(res != ESP_OK){
      break;
    }
    
    wsLive_val++;
    if (wsLive_val > 999) wsLive_val = 0;
  }
  /* ---------------------------------------- */
  return res;
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ cmd handler function to be called during GET or uri request. */
static esp_err_t cmd_handler(httpd_req_t *req){
  char*  buf;
  size_t buf_len;
  char variable[32] = {0,};
   
  buf_len = httpd_req_get_url_query_len(req) + 1;
  if (buf_len > 1) {
    buf = (char*)malloc(buf_len);
    if(!buf){
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }
    if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
      if (httpd_query_key_value(buf, "go", variable, sizeof(variable)) == ESP_OK) {
      } else {
        free(buf);
        httpd_resp_send_404(req);
        return ESP_FAIL;
      }
    } else {
      free(buf);
      httpd_resp_send_404(req);
      return ESP_FAIL;
    }
    free(buf);
  } else {
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  int res = 0;

  if(!strcmp(variable, "forward")) {
    Serial.println("Forward");
    digitalWrite(MOTOR_1_PIN_1, 1);
    digitalWrite(MOTOR_1_PIN_2, 0);
    digitalWrite(MOTOR_2_PIN_1, 1);
    digitalWrite(MOTOR_2_PIN_2, 0);
  }
  else if(!strcmp(variable, "left")) {
    Serial.println("Left");
    digitalWrite(MOTOR_1_PIN_1, 0);
    digitalWrite(MOTOR_1_PIN_2, 1);
    digitalWrite(MOTOR_2_PIN_1, 1);
    digitalWrite(MOTOR_2_PIN_2, 0);
  }
  else if(!strcmp(variable, "right")) {
    Serial.println("Right");
    digitalWrite(MOTOR_1_PIN_1, 1);
    digitalWrite(MOTOR_1_PIN_2, 0);
    digitalWrite(MOTOR_2_PIN_1, 0);
    digitalWrite(MOTOR_2_PIN_2, 1);
  }
  else if(!strcmp(variable, "backward")) {
    Serial.println("Backward");
    digitalWrite(MOTOR_1_PIN_1, 0);
    digitalWrite(MOTOR_1_PIN_2, 1);
    digitalWrite(MOTOR_2_PIN_1, 0);
    digitalWrite(MOTOR_2_PIN_2, 1);
  }
  else if(!strcmp(variable, "stop")) {
    Serial.println("Stop");
    digitalWrite(MOTOR_1_PIN_1, 0);
    digitalWrite(MOTOR_1_PIN_2, 0);
    digitalWrite(MOTOR_2_PIN_1, 0);
    digitalWrite(MOTOR_2_PIN_2, 0);
  }
  else { 
    Serial.print("Incoming command : ");
    Serial.println(variable);
    Serial.println();
  }
  String getData = String(variable);
  String resultData = getValue(getData, ',', 0);

  /* ---------------------------------------- Controlling the LEDs on the ESP32 Cam board with PWM. */
  // Example :
  // Incoming command = S,10
  // S = Slider
  // 10 = slider value
  // I set the slider value range from 0 to 20.
  // Then the slider value is changed from 0 - 20 or vice versa to 0 - 255 or vice versa.
  if (resultData == "S") {
    resultData = getValue(getData, ',', 1);
    ledcSetup(4, 150, 8);
    ledcAttachPin(LED_OnBoard, 4);  
    ledcWrite(LED_OnBoard,map(resultData.toInt(), 0, 20, 0, 255));
  }
  if (resultData == "M") {
    ledcSetup(6, 1000, 8);
    ledcAttachPin(PWM_MOTORS_PIN, 6);  
    ledcWrite(PWM_MOTORS_PIN,resultData.toInt());
  }
  /* ---------------------------------------- */

  /* ---------------------------------------- Clean the result of reading the QR Code. */
  // Incoming Command = B,clr
  // B = Button
  // clr = Command to clean the results of reading the QR Code.
  if (resultData == "B") {
    resultData = getValue(getData, ',', 1);
    if (resultData == "clr") {
      QRCodeResult = "";
    }
  }
  /* ---------------------------------------- */
  
  if(res){
    return httpd_resp_send_500(req);
  }

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  return httpd_resp_send(req, NULL, 0);
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ qrcoderslt handler function to be called during GET or uri request. */
static esp_err_t qrcoderslt_handler(httpd_req_t *req){
  if (QRCodeResult != "Decoding FAILED") QRCodeResultSend = QRCodeResult;
  httpd_resp_send(req, QRCodeResultSend.c_str(), HTTPD_RESP_USE_STRLEN);
  return ESP_OK;
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ Subroutine for starting the web server / startCameraServer. */
void startCameraWebServer(){
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = index_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t cmd_uri = {
    .uri       = "/action",
    .method    = HTTP_GET,
    .handler   = cmd_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t qrcoderslt_uri = {
    .uri       = "/getqrcodeval",
    .method    = HTTP_GET,
    .handler   = qrcoderslt_handler,
    .user_ctx  = NULL
  };

  httpd_uri_t stream_uri = {
    .uri       = "/stream",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };
    
  if (httpd_start(&index_httpd, &config) == ESP_OK) {
      httpd_register_uri_handler(index_httpd, &index_uri);
      httpd_register_uri_handler(index_httpd, &cmd_uri);
      httpd_register_uri_handler(index_httpd, &qrcoderslt_uri);
  }

  config.server_port += 1;
  config.ctrl_port += 1;  
  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
      httpd_register_uri_handler(stream_httpd, &stream_uri);
  }

  Serial.println();
  Serial.println("Camera Server started successfully");
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
  Serial.println();
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ VOID SETUP() */
void setup() {
  // put your setup code here, to run once:

  // Disable brownout detector.
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  pinMode(MOTOR_1_PIN_1, OUTPUT);
  pinMode(MOTOR_1_PIN_2, OUTPUT);
  pinMode(MOTOR_2_PIN_1, OUTPUT);
  pinMode(MOTOR_2_PIN_2, OUTPUT);
  
  /* ---------------------------------------- Init serial communication speed (baud rate). */
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  /* ---------------------------------------- */

  pinMode(LED_OnBoard, OUTPUT);
  pinMode(PWM_MOTORS_PIN,OUTPUT);


  /* ---------------------------------------- Camera configuration. */
  Serial.println("Start configuring and initializing the camera...");
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 15;
  config.fb_count = 1;
  
  #if defined(CAMERA_MODEL_ESP_EYE)
    pinMode(13, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
  #endif

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  }
  
  sensor_t * s = esp_camera_sensor_get();
  s->set_framesize(s, FRAMESIZE_QVGA);
  
  Serial.println("Configure and initialize the camera successfully.");
  Serial.println();
  /* ---------------------------------------- */

  /* ---------------------------------------- Connect to Wi-Fi. */
  WiFi.mode(WIFI_STA);
  Serial.println("------------");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  /* ::::::::::::::::: The process of connecting ESP32 CAM with WiFi Hotspot / WiFi Router. */
  /*
   * The process timeout of connecting ESP32 CAM with WiFi Hotspot / WiFi Router is 20 seconds.
   * If within 20 seconds the ESP32 CAM has not been successfully connected to WiFi, the ESP32 CAM will restart.
   * I made this condition because on my ESP32-CAM, there are times when it seems like it can't connect to WiFi, so it needs to be restarted to be able to connect to WiFi.
   */
  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    digitalWrite(LED_OnBoard, HIGH);
    delay(250);
    digitalWrite(LED_OnBoard, LOW);
    delay(250);
    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
    if(connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  digitalWrite(LED_OnBoard, LOW);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("------------");
  Serial.println("");
  /* ::::::::::::::::: */
  /* ---------------------------------------- */

  // Start camera web server
  startCameraWebServer(); 
  
  // Calls the createTaskQRCodeReader() subroutine.
  createTaskQRCodeReader();
}
/* ________________________________________________________________________________ */

void loop() {
  // put your main code here, to run repeatedly:

  /* ---------------------------------------- Condition to check whether the index page is being accessed or not. */
  // If the index page is not accessed, the loop will call the "QRCodeReader()" Subroutine.
  // If the index page is accessed, the Loop will not call the "QRCodeReader()" Subroutine.
  // I created this condition because the camera cannot be accessed at the same time by the index page and the "QRCodeReader()" subroutine.
  // Meanwhile, I designed this project to be able to read the QR Code when the index page is accessed or the index page is not being accessed.
  // Even if I have to share camera data, I haven't found the best way to share camera data to the "QRCodeReader()" sub routine and stream function.
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ws_run == true) {
      if (get_wsLive_val == true) {
        last_wsLive_val = wsLive_val;
        get_wsLive_val = false;
      }
   
      get_wsLive_interval++;
      if (get_wsLive_interval > 2) {
        get_wsLive_interval = 0;
        get_wsLive_val = true;
        if (wsLive_val == last_wsLive_val) {
          ws_run = false;
          last_wsLive_val = 0;
          createTaskQRCodeReader();
        }
      }
    }
  }
  /* ---------------------------------------- */
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ create "QRCodeReader_Task" using the xTaskCreatePinnedToCore() function */
void createTaskQRCodeReader() {
  xTaskCreatePinnedToCore(
             QRCodeReader,          /* Task function. */
             "QRCodeReader_Task",   /* name of task. */
             10000,                 /* Stack size of task */
             NULL,                  /* parameter of the task */
             1,                     /* priority of the task */
             &QRCodeReader_Task,    /* Task handle to keep track of created task */
             0);                    /* pin task to core 0 */
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ QRCodeReader() */
void QRCodeReader( void * pvParameters ){
  Serial.print("QRCodeReader running on core ");
  Serial.println(xPortGetCoreID());

  while(!ws_run){
      camera_fb_t * fb = NULL;
      q = quirc_new();
      
      if (q == NULL){
        Serial.print("can't create quirc object\r\n");  
        continue;
      }
      
      fb = esp_camera_fb_get();
      if (!fb)
      {
        Serial.println("Camera capture failed (QRCodeReader())");
        continue;
      }
      
      quirc_resize(q, fb->width, fb->height);
      image = quirc_begin(q, NULL, NULL);
      memcpy(image, fb->buf, fb->len);
      quirc_end(q);
      
      int count = quirc_count(q);
      if (count > 0) {
        //Serial.println(count);
        quirc_extract(q, 0, &code);
        err = quirc_decode(&code, &data);
    
        if (err){
          QRCodeResult = "Decoding FAILED";
          Serial.println(QRCodeResult);
        } else {
          Serial.printf("Decoding successful:\n");
          dumpData(&data);
        } 
        Serial.println();
      } 
      
      esp_camera_fb_return(fb);
      fb = NULL;
      image = NULL;  
      quirc_destroy(q);
  }
}
/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ Function to display the results of reading the QR Code on the serial monitor. */
void dumpData(const struct quirc_data *data)
{
  Serial.printf("-Version: %d\n", data->version);
  Serial.printf("-ECC level: %c\n", "MLHQ"[data->ecc_level]);
  Serial.printf("-Mask: %d\n", data->mask);
  Serial.printf("-Length: %d\n", data->payload_len);
  Serial.printf("-Payload: %s\n", data->payload);
  
  QRCodeResult = (const char *)data->payload;
  increment(QRCodeResult);
}
/*
void increment(String QR_res){
  Serial.println("increment");
    if(QR_res.startsWith("product_")){
        for(uint8_t i; i<size_produkt;i++){
      if(QR_res.substring(8) ==  name_produkt[i]){
        Serial.println("inc mas");
        num_produkt[i] = num_produkt[i]+1;
      }
      else {
        name_produkt[last_redukt] = QR_res.substring(8);
        num_produkt[last_redukt]=1;
        last_redukt++;
        Serial.println("OK");
        
      }/*
      for(uint8_t i; i < size_produkt; i++){
        Serial.print(name_produkt[1]+" ");
      }
      Serial.println();
      for(uint8_t i; i < size_produkt; i++){
        Serial.print(num_produkt[1]+" ");
      }
      *//*
      return; 
    }

  }
     
}
*/


unsigned int  num_yablok;
unsigned int  num_morkov;
unsigned int  num_maslo;
unsigned int  num_limonad;
unsigned int  num_kartoshka;
unsigned int  num_baton;
unsigned int  num_yogurt;
unsigned int  num_shokolad;
unsigned int  num_kapusta;
unsigned int  num_apelsin;
unsigned int  num_redis;
unsigned int  num_kolbasa;

void increment(String QR_res){
  Serial.println("increment");
  if(QR_res== "product_yabloko")    num_yablok++;
  if(QR_res== "product_morkov")     num_morkov++;
  if(QR_res== "product_maslo")      num_maslo++;
  if(QR_res== "product_limonad")    num_limonad++;
  if(QR_res== "product_kartoshka")  num_kartoshka++;
  if(QR_res== "product_baton")      num_baton++;
  if(QR_res== "product_yogurt")     num_yogurt++;
  if(QR_res== "product_shokolad")   num_shokolad++;
  if(QR_res== "product_kapusta")    num_kapusta++;
  if(QR_res== "product_apelsin")    num_apelsin++;
  if(QR_res== "product_redi")       num_yablok++;
  if(QR_res== "product_kolbasa")    num_kolbasa++;
  delay(2000);
}

/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ Subroutine to execute commands based on the results of reading the QR Code. */
/*
void cmd_execution() {
  if (QRCodeResult == "LED BLUE ON") digitalWrite(LED_Blue, HIGH);
  if (QRCodeResult == "LED BLUE OFF") digitalWrite(LED_Blue, LOW);
  if (QRCodeResult == "LED GREEN ON") digitalWrite(LED_Green, HIGH);
  if (QRCodeResult == "LED GREEN OFF") digitalWrite(LED_Green, LOW);
}

/* ________________________________________________________________________________ */

/* ________________________________________________________________________________ String function to process the data command */
// I got this from : https://www.electroniclinic.com/reyax-lora-based-multiple-sensors-monitoring-using-arduino/
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
/* ________________________________________________________________________________ */
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
