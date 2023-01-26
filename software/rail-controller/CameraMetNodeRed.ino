#include "esp_camera.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//=============================={CAMERA VAR}================================
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"
#define LED_BUILTIN 4
//=============================={SERVO VAR}================================
//Servo pin
Servo dummyServo1; // 2 dummy servos.
Servo dummyServo2; //voor ESP32Servo library niet gaat interferen met de timer van camera.
Servo servoPan;
Servo servoTilt;

byte pinPan = 14;
byte pinTilt = 15;
byte panPositie = 0;
byte tiltPositie = 0;

//=============================={MQTT en WIFI VAR}================================
//WIFI config
const char* ssid = "WwiFi";
const char* password = "123456789";

//MQTT config
bool useMQTT = true;
const char* mqttServer = "172.16.157.225";
const int mqtt_port = 1884;
const char* HostName = "ESP32camENServo";

String bericht;
String TOPIC;
const char* topic_FOTO = "Farmlab2/slider/Foto";
const char* topic_FOTO_KIJK = "Farmlab2/slider/KijkFoto";
const char* topic_FLASH = "Farmlab2/slider/Flash";
const char* topic_Pan = "Farmlab2/slider/pan";
const char* topic_Tilt = "Farmlab2/slider/tilt";
const char* topic_TiltPos = "Farmlab2/slider/tiltPos";
const char* topic_PanPos = "Farmlab2/slider/panPos";
const char* topic_Foto_auto = "Farmlab2/slider/FotoAuto";
const int MAX_PAYLOAD = 60000;

bool flash;

WiFiClient espClient;
PubSubClient client(espClient);

enum toestandEnum {
  FOTO,
  FLASH,
  FotoAUTO,
  PAN,
  TILT,
  idelModus
};

toestandEnum toestand;

void startCameraServer();
//VAR
int b = 0;
const int DELAY_AFTER_MOVING_SERVO = 3000; // delay in milliseconds after moving the servo
const int DELAY_AFTER_TAKING_PHOTO = 2000; // delay in milliseconds after taking a photo



byte arrNum = 0;
int posCam[3] = {120, 90, 60};
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  //========Serial Communicatie========
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  //Flash as an output
  pinMode(LED_BUILTIN, OUTPUT);

  //========Servo setup========
  servoPan.setPeriodHertz(50);    // standard 50 hz servo
  servoTilt.setPeriodHertz(50);  // standard 50 hz servo
  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep
  dummyServo1.attach(2, 1000, 2000);
  dummyServo2.attach(13, 1000, 2000);
  servoPan.attach(pinPan, 1000, 2000);
  servoTilt.attach(pinTilt, 1000, 2000);
  servoPan.write(90);
  servoTilt.write(90);
  camera_config();
  //========WIFI EN MQTT setup========
  setup_wifi();
  client.setServer(mqttServer, mqtt_port);
  client.setBufferSize (MAX_PAYLOAD); //This is the maximum payload length
  client.setCallback(callback);
  reconnect();
}

void loop() {
  client.loop();
  switch (toestand) {
    case FOTO:
      take_picture();
      toestand = idelModus;
      break;
    case FLASH:
      set_flash();
      toestand = idelModus;
      break;
    case FotoAUTO:

      servoPan.write(posCam[arrNum]);
      arrNum++;
      if (arrNum > 2) {
        arrNum = 0;
      }

      delay(DELAY_AFTER_MOVING_SERVO);
      take_picture();
      delay(DELAY_AFTER_TAKING_PHOTO);
      Serial.println(arrNum);
      b = 0;

      toestand = idelModus;
      break;
    case PAN:
      panPositie = bericht.toInt();
      Serial.print("Pan pos: ");
      Serial.println(panPositie);
      servoPan.write(panPositie);
      servoPan.write(panPositie);
      toestand = idelModus;
      break;
    case TILT:
      tiltPositie = bericht.toInt();
      Serial.print("Tilt pos: ");
      Serial.println(tiltPositie);
      servoTilt.write(tiltPositie);
      toestand = idelModus;
      break;
    default:
      break;
  }
}

//============================{FUNCTION SETUP}=============================

//=============={WIFI}==============

void setup_wifi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  startCameraServer();
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");

}
//================{MQTT}==================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(HostName)) {
      Serial.println("connected");
      client.subscribe(topic_FOTO);
      client.subscribe(topic_FLASH);
      client.subscribe(topic_Pan);
      client.subscribe(topic_Tilt);
      client.subscribe(topic_Foto_auto);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void sendMQTT(const uint8_t * buf, uint32_t len) {
  Serial.println("Sending picture...");
  if (len > MAX_PAYLOAD) {
    Serial.print("Picture too large, increase the MAX_PAYLOAD value");
  } else {
    Serial.print("Picture sent ? : ");
    Serial.println(client.publish(topic_FOTO_KIJK, buf, len, false));
  }

}

void callback(String topic, byte* message, unsigned int length) {
  Serial.println(topic);
  bericht = "";
  TOPIC = topic;
  for (int i = 0; i < length; i++) {
    bericht += (char)message[i];
  }

  if (TOPIC == topic_FOTO)
    toestand = FOTO;

  if (TOPIC == topic_FLASH)
    toestand = FLASH;

  if (TOPIC == topic_Foto_auto)
    toestand = FotoAUTO;

  if (TOPIC == topic_Pan)
    toestand = PAN;

  if (TOPIC == topic_Tilt)
    toestand = TILT;
}

//========================{Camera foto en flash}================================

void camera_config() {
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
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  flash = true;

  // Not used in our project
#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  //initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);//flip it back
    s->set_brightness(s, 1);//up the blightness just a bit
    s->set_saturation(s, -2);//lower the saturation
  }
  //drop down frame size for higher initial frame rate
  s->set_framesize(s, FRAMESIZE_XGA);

  // Not used in our project
#if defined(CAMERA_MODEL_M5STACK_WIDE)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif
}


void take_picture() {
  camera_fb_t * fb = NULL;
  flash = true;
  if (flash) {
    digitalWrite(LED_BUILTIN, HIGH);
  };
  Serial.println("Taking picture");
  fb = esp_camera_fb_get(); // used to get a single picture.
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  Serial.println("Picture taken");
  digitalWrite(LED_BUILTIN, LOW);
  sendMQTT(fb->buf, fb->len);
  esp_camera_fb_return(fb); // must be used to free the memory allocated by esp_camera_fb_get().

}

void set_flash() {
  flash = !flash;
  Serial.print("Setting flash to ");
  Serial.println (flash);
  if (!flash) {
    for (int i = 0; i < 6; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
  if (flash) {
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(500);
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
    }
  }
}