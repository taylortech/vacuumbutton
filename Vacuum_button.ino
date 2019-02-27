// working d. bodnar12-25-2017  

/*
   static const uint8_t D0   = 16;
  static const uint8_t D1   = 5;
  static const uint8_t D2   = 4;
  static const uint8_t D3   = 0;
  static const uint8_t D4   = 2;
  static const uint8_t D5   = 14;
  static const uint8_t D6   = 12;
  static const uint8_t D7   = 13;
  static const uint8_t D8   = 15;
  static const uint8_t RX   = 3;
  static const uint8_t TX = 1;
*/

#include <Servo.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"
unsigned long previousMillis = 0;
const long interval = 1000;
boolean connectWifi();
void vacuumOn();
void vacuumOff();
// Change this before you flash
const char* ssid = "Taylor Castle-2.4";
const char* password = "ashtonrobert";
boolean wifiConnected = false;
UpnpBroadcastResponder upnpBroadcastResponder;
Switch *vacuum = NULL;
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);  // set onboard LED as output
  Serial.begin(9600);
  wifiConnected = connectWifi();
  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();
    vacuum = new Switch("vacuum ", 80, vacuumOn, vacuumOff);
    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*vacuum);
  }
}

void loop()
{
  if (wifiConnected) {
    upnpBroadcastResponder.serverLoop();
    vacuum->serverLoop();
  }
  // unsigned long currentMillis = millis();
  if (millis() - previousMillis >= interval) {
    previousMillis = millis();
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
  }
}

void vacuumOn() {
  myservo.attach(14);  // attaches the servo on pin 12 to the servo object
  Serial.print("Switch 1 turn on ...");
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
    for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  myservo.detach();  // attaches the servo on pin 12 to the servo object
}

void vacuumOff() {
  Serial.print("Switch 1 turn off ...");
}


// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");
  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) {
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
