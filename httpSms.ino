#define AUTH_TOKEN ""


#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <SoftwareSerial.h>
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

ESP8266WebServer server(80);
SoftwareSerial mySerial(14, 12, false, 128);

const char* ssid = "RTK-29(9)";
const char* password = "77864163135";

void simPrint(String command){
  mySerial.print(command);
  delay(500);
  while (mySerial.available()){
    Serial.write(mySerial.read());
  }
  Serial.println();
}

void simPrintln(String command){
  simPrint(command+"\n");
}

void sendSMS(String text, String number){

  Serial.println("Sending...");

  Serial.print("Message:");
  Serial.println(text);
  Serial.print("Number:");
  Serial.println(number);
  
  simPrintln("AT+CMGF=1");
  simPrintln("AT+CMGS=\""+number+"\"\n"); // Enter your phone number here 
(prefix country code)
  simPrint(text); // Enter your message here
  mySerial.write(26);
  Serial.println("Sent");
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRootPath);
  server.begin();
  Serial.println("Server listening");

  mySerial.begin(57600);
  simPrintln("AT");
  Serial.println("Connected to SIM module");
}

void loop() {
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
  server.handleClient();
}

void handleRootPath() {
  if (server.hasArg("authToken") == false) {
    server.send(400, "text/plain", "authToken in empty");
    return;
  }
  if (server.hasArg("phone") == false) {
    server.send(400, "text/plain", "phone is empty");
    return;
  }
  if (server.hasArg("msg") == false) {
    server.send(400, "text/plain", "msg is empty");
    return;
  }

  String authToken = server.arg("authToken");
  String phone = server.arg("phone");
  String msg = server.arg("msg");

  if(authToken != AUTH_TOKEN){
    server.send(401, "text/plain", "Invalid authToken");
    return;
  }

  sendSMS(msg, phone);
  server.send(200, "text/plain", authToken);
}
