

// Sample code for ESP32  //

#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "Nong-WiFi"; //change this to your SSID
const char* password = "abcde12345"; //change this to your PASSWORD

const char* host = "http://fitfarm.herokuapp.com/bot.php";
#define APPID   "FitfarmLinebot"     //change this to your APPID
#define KEY     "ToLd5f0617sX7LB"     //change this to your KEY
#define SECRET  "uskIJuEFrKivJwTzjLrljz6ZY"     //change this to your SECRET

#define ALIAS   "ESP32"                    //set name of device

#define TRIGGER 5
#define ECHO    4
// NodeMCU Pin D1 > TRIGGER | Pin D2 > ECHO

#include <Servo.h> // including servo library.

Servo servo_1; // Giving name to servo.

WiFiClient client;
String uid = "";
int timer = 0;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("LINE MSG is");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
  String msgLINE = (char *)msg;
  if ( msgLINE == "ON" || msgLINE == "On" || msgLINE == "on" || msgLINE == "รดน้ำ" || msgLINE == "1") {
    digitalWrite(2, LOW);         // LED on
    send_json("เป่าเปารดน้ำให้แล้วฮ่ะ");
  }else if ( msgLINE == "OFF" || msgLINE == "Off"  || msgLINE == "off" || msgLINE == "ปิดน้ำ" || msgLINE == "2") {
    digitalWrite(2, HIGH);          // LED off
    send_json("เป่าเปาปิดน้ำให้แล้วฮ่ะ");
  }else if ( msgLINE == "กิน" || msgLINE == "อาหาร"  || msgLINE == "เอาไป" || msgLINE == "จ้า" || msgLINE == "3") {
    servo_1.write (180);  // servo will move to 90 degree angle.
    delay (3000);
    servo_1.write (0); 
    send_json("มะหมาได้กินแล้วฮ่ะ");
  }
  else{
    send_json("เป่าเปาไม่เข้าใจคำสั่งฮ่ะ");
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

  servo_1.attach(0); // Attaching Servo to D3
  servo_1.write (0); // Servo will move to 45 degree angle.

  pinMode(2, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

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
  long duration, distance;
  digitalWrite(TRIGGER, LOW);  
  delayMicroseconds(2); 
  
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(TRIGGER, LOW);
  duration = pulseIn(ECHO, HIGH);
  distance = (duration/2) / 29.1;
  
  Serial.print(distance);
  Serial.println("Centimeter:");
  
  if (microgear.connected()) {
  //  Serial.println("NETPIE is connecting!");
    microgear.loop();
    timer = 0;
    if(distance > 0 && distance < 10){
       send_json("เป่าเปาหิวข้าวแล้วฮ่ะ");
       send_json("ให้อาหารหน่อย");
       delay(5000);
    }
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
