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

#define ALIAS   "NodeMCU1" //set name of drvice
#define TargetWeb "switch" //set target name of web

WiFiClient client;
String uid = "";
int timer = 0;
int mockSensor = 0;
bool conn = false;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) { // 
    Serial.print("Incoming message -->");
    msg[msglen] = '\0';
Serial.println((char *)msg);
    if(*(char *)msg == '1'){
        digitalWrite(16, HIGH);   // LED on
        //microgear.chat(TargetWeb,"1");
        //send_data("ESP_LED_ON");
        send_json("Level 1");
    }else if(*(char *)msg == '2'){
          digitalWrite(16, LOW);   // LED on
        //microgear.chat(TargetWeb,"1");
        //send_data("ESP_LED_ON");
        send_json("Level 2");
    }
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    microgear.setName(ALIAS);
}


void setup() {
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);

    Serial.begin(115200);
    Serial.println("Starting...");

    pinMode(16, OUTPUT);
  
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
     digitalWrite(16, HIGH);   // LED on
}

void send_json(String data){
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
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin(host);      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.POST(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                                        //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
}
void loop() {

    if(conn){
      if(mockSensor == 1000){
        send_json("sensor handle!!");
        mockSensor = 0;
      }
      mockSensor++;
    }
     
    if (microgear.connected()) {
        Serial.println("..."); 
        Serial.println(mockSensor); 
        microgear.loop();
        timer = 0;
        conn = true;
    }
    else {
        Serial.println("connection lost, reconnect...");
        conn = false;
        if (timer >= 5000) {
            microgear.connect(APPID); 
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}
