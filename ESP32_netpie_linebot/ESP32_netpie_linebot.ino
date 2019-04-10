

// Sample code for ESP32  //

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "Nong-WiFi"; //change this to your SSID
const char* password = "abcde12345"; //change this to your PASSWORD

const char* host = "http://fitfarm.herokuapp.com/bot.php";//change this to your linebot server ex.http://numpapick-linebot.herokuapp.com/bot.php
#define APPID   "FitfarmLinebot"     //change this to your APPID
#define KEY     "ToLd5f0617sX7LB"     //change this to your KEY
#define SECRET  "uskIJuEFrKivJwTzjLrljz6ZY"     //change this to your SECRET

#define ALIAS   "ESP32"                    //set name of device

WiFiClient client;
String uid = "";
int timer = 0;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("LINE MSG is");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
  String msgLINE = (char *)msg;
  if ( msgLINE == "ON" || msgLINE == "On" || msgLINE == "on" ) {
    digitalWrite(2, HIGH);         // LED on
    send_json("LED Turn on");
  }
  if ( msgLINE == "OFF" || msgLINE == "Off"  || msgLINE == "off" ) {
    digitalWrite(2, LOW);          // LED off
    send_json("LED turn off");
  }
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("NETPIE is connected");
  microgear.setName(ALIAS);
}

void send_json(String data) {
  StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["ESP"] = data;

  JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
  values.add(20); //Add value to array
  values.add(21); //Add value to array
  values.add(23); //Add value to array


  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println(JSONmessageBuffer);

  HTTPClient http;                                    //Declare object of class HTTPClient

  http.begin(host);                                   //Specify request destination
  http.addHeader("Content-Type", "application/json"); //Specify content-type header

  int httpCode = http.POST(JSONmessageBuffer);        //Send the request
  String payload = http.getString();                  //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();                 //Close connection
}

void setup() {
  microgear.on(MESSAGE, onMsghandler);
  microgear.on(CONNECTED, onConnected);

  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(2, OUTPUT);

  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  microgear.init(KEY, SECRET, ALIAS);
  microgear.connect(APPID);
  digitalWrite(2, HIGH);   // LED on
}

void loop() {
  if (microgear.connected()) {
  //  Serial.println("NETPIE is connecting!");
    microgear.loop();
    timer = 0;
  }
  else {
    Serial.println("NETPIE is reconnecting...");
    if (timer >= 5000) {
      microgear.connect(APPID);
      timer = 0;
    }
    else timer += 100;
  }
  delay(100);
}
