#include <SoftwareSerial.h>

SoftwareSerial mySerial(14, 12, false, 128); //Define hardware connections


void simPrint(String command)
{
  mySerial.print(command);
  delay(500);
 
  while (mySerial.available())
  {
    Serial.write(mySerial.read());
  }
  Serial.println();
}

void simPrintln(String command){
  simPrint(command+"\n");
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // set the data rate for the SoftwareSerial port
  mySerial.begin(57600);
  simPrintln("AT");
  sendSMS("Test message", "+79003901398");

}


void sendSMS(String text, String number){

  Serial.println("Sending...");

  Serial.print("Message:");
  Serial.println(text);
  Serial.print("Number:");
  Serial.println(number);
  
  simPrintln("AT+CMGF=1");
  simPrintln("AT+CMGS=\""+number+"\"\n"); // Enter your phone number here (prefix country code)
  simPrint(text); // Enter your message here
  mySerial.write(26);
  Serial.println("Sent");
}

void loop() { // run over and over
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}