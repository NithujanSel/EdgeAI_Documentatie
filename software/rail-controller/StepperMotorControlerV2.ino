#include "AccelStepper.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
//========================{WIFI VAR}=====================
WiFiClient espClient;
PubSubClient client(espClient);

//==========================={MOTOR VAR}===========================
#define dirPin 26
#define stepPin 27
#define motorInterfaceType 1
AccelStepper Slider(motorInterfaceType, stepPin, dirPin); // motorInterface,stepPin,directionPin

int motorSpeed = 610;
volatile bool motorStart = false;
volatile bool stopWhile = false;
int posLoc = 0;
int homePos = 0;
int currentPos = 0;
int posVersterking = 100;
//============================{MQTT VAR}=============================
const char* ssid = "WwiFi";
const char* password = "123456789";
const char* mqtt_server = "172.16.157.225";
const int mqtt_port = 1884;

String TOPIC;
String bericht;
String getCordiante;
//========================{EINDELOOPSCHAKELAAR VAR}=====================
byte eideloopschakelaarLinks = 4;//L
byte eideloopschakelaarRechts = 5;//5

//========================{VAR}=====================
int getPos[50];
int arraySize = 0;
int autoVerder = 1;
int breakLoop = 0;

enum enToestand {
  SPEED,
  START,
  LEFT,
  RIGHT,
  SET_ZERO,
  GO_HOME,
  GO_TO_POS,
  ARRAY_LOOP,
  MAKE_ARRAY,
  FEEDBACK,
  IDLEModus
};


int motorRichting = 1;
enToestand toestand;

//=============={INTERUPT EINDELOOP}==============
void IRAM_ATTR stoppenLinks() {//L
  //motorStart = false;
  stopWhile = true;
  //Slider.stop();
  toestand = GO_HOME;
}

void IRAM_ATTR stoppenRechts() {//R
  //motorStart = false;
  stopWhile = true;
  //Slider.stop();
  toestand = GO_HOME;
}



void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  //============================{SERIAL SETUP}=============================
  Serial.begin(115200);
  //============================{MOTOR SETUP}=============================
  Slider.setMaxSpeed(1000);
  //Slider.setAcceleration(1);
  //========================{EINDELOOPSCHAKELAAR SETUP}=====================
  pinMode(eideloopschakelaarLinks, INPUT);
  pinMode(eideloopschakelaarRechts, INPUT);
  attachInterrupt(eideloopschakelaarLinks, stoppenLinks, RISING);
  attachInterrupt(eideloopschakelaarRechts, stoppenRechts, RISING);

  toestand = IDLEModus;
  //============================{WIFI em MQTT SETUP}=============================
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  memset(getPos, 0, sizeof(getPos));
  reconnect();

}


void loop() {
  //============================{MQTT LOOP}=============================
  client.loop();

  switch (toestand) {
    case SPEED:
      motorSpeed = bericht.toInt();
      toestand = IDLEModus;
      break;
    case START:
      motorStart = (bericht == "true");
      toestand = IDLEModus;
      break;
    case LEFT:
      if (motorStart) {
        Slider.setSpeed(-motorSpeed);
        Slider.runSpeed();
      }
      break;
    case RIGHT:
      if (motorStart) {
        Slider.setSpeed(motorSpeed);
        Slider.runSpeed();
      }
      //      Serial.println("rechts");
      //      Serial.println(Slider.currentPosition());
      //      delay(10000);
      //      toestand = IDLEModus;
      break;
    case SET_ZERO:
      Slider.setCurrentPosition(0);
      homePos = Slider.currentPosition();
      toestand = IDLEModus;
      break;
    case GO_HOME:
      if (Slider.currentPosition() > 0)
        motorRichting = -1;

      if (Slider.currentPosition() < 0)
        motorRichting = 1;

      while ((Slider.currentPosition() > homePos) && motorStart)
      {
        Slider.setSpeed(motorRichting * motorSpeed);
        Slider.runSpeed();
        Serial.println(Slider.currentPosition());
        currentPos = Slider.currentPosition();
      }
      Serial.println("Home");
      stopWhile = false;
      toestand = IDLEModus;
      break;
    case GO_TO_POS:
      posLoc = bericht.toInt() * posVersterking;
      while ((Slider.currentPosition() < posLoc && motorStart) && stopWhile == false)
      {
        Slider.setSpeed(motorSpeed);
        Slider.runSpeed();
        Serial.println(Slider.currentPosition());
      }

      while ((Slider.currentPosition() > posLoc && motorStart) && stopWhile == false)
      {
        Slider.setSpeed(-motorSpeed);
        Slider.runSpeed();
        Serial.println(Slider.currentPosition());
      }
      toestand = IDLEModus;
      break;
    case MAKE_ARRAY:
      getCordiante = bericht;
      convert_array(getCordiante);
      Serial.print("Array length: ");
      Serial.println(String(arraySize));
      toestand = IDLEModus;
      break;
    case  ARRAY_LOOP:
      while (Slider.currentPosition() < (getPos[breakLoop] * posVersterking)) {
        //Slider.run();
        Slider.setSpeed(motorSpeed);
        Slider.runSpeed();
        Serial.println(Slider.currentPosition());
        //currentPos = Slider.currentPosition();
      }

      breakLoop++;
      if (breakLoop <= arraySize) {
        Serial.println("naar idle");
        Serial.println("send Foto auto topic");
        client.publish("Farmlab2/slider/FotoAuto", "1");
        toestand = IDLEModus;
        break;
      }
      else {
        breakLoop = 0;
        toestand = GO_HOME;
        break;
      }

      Serial.println("Ga verder");
      //delay(3000);

      break;
    case FEEDBACK:
      toestand = ARRAY_LOOP;
      break;
    default:
      Slider.stop();
      break;
  }
  //toestand = IDLEModus;
}





//============================{FUNCTION SETUP}=============================

//=============={WIFI}==============

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}
//=============={MQTT}==============

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Stepper";
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("Farmlab2/stepper/links");
      client.subscribe("Farmlab2/stepper/rechts");
      client.subscribe("Farmlab2/stepper/start");
      client.subscribe("Farmlab2/stepper/speed");
      client.subscribe("Farmlab2/stepper/postPos");
      client.subscribe("Farmlab2/stepper/postPosList");
      client.subscribe("Farmlab2/stepper/set0punt");
      client.subscribe("Farmlab2/stepper/beginPunt");
      client.subscribe("Farmlab2/auto/maakFotoOm8");
      client.subscribe("Farmlab2/auto/maakFotoOm12");
      client.subscribe("Farmlab2/auto/maakFotoOm18");
      client.subscribe("Farmlab2/auto/Verder");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void callback(String topic, byte * message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  Serial.println();
  bericht = "";
  TOPIC = topic;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    bericht += (char)message[i];
  }
  Serial.println();

  if (TOPIC == "Farmlab2/auto/Verder")
    toestand = FEEDBACK;
  if (TOPIC == "Farmlab2/stepper/speed")
    toestand = SPEED;

  if (TOPIC == "Farmlab2/stepper/start")
    toestand = START;

  if (TOPIC == "Farmlab2/stepper/links")
    toestand = LEFT;

  if (TOPIC == "Farmlab2/stepper/rechts")
    toestand = RIGHT;

  if (TOPIC == "Farmlab2/stepper/set0punt")
    toestand = SET_ZERO;

  if (TOPIC == "Farmlab2/stepper/beginPunt")
    toestand = GO_HOME;

  if (TOPIC == "Farmlab2/stepper/postPos")
    toestand = GO_TO_POS;

  if (TOPIC == "Farmlab2/stepper/postPosList")
    toestand = MAKE_ARRAY;

  if (TOPIC == "Farmlab2/auto/maakFotoOm8" || TOPIC == "Farmlab2/auto/maakFotoOm12" || TOPIC == "Farmlab2/auto/maakFotoOm18")
    toestand = ARRAY_LOOP;

}

//=============={Conver to Array}==============

void convert_array(String str) {
  int str_length = str.length();
  int i = 0;
  int j = 0;
  //traversing the string
  while (i < str_length) {
    // spatie skip
    while (i < str_length && str[i] == ' ') {
      i++;
    }

    // einde van strin dan stopen
    if (i == str_length) {
      break;
    }

    // van string int maken
    int val = 0;
    while (i < str_length && isdigit(str[i])) {
      val = val * 10 + (str[i] - '0');
      i++;
    }

    // 2 extra coordianten bijzetten voor foto
    getPos[j] = val;
    int tijdeD = getPos[j];
    getPos[j + 1] = tijdeD + 1;
    getPos[j + 2] = tijdeD + 2;
    j += 3;

    // als comma is skipen
    if (i < str_length && str[i] == ',') {
      i++;
    }
  }

  arraySize = j;
}