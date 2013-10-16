#include <Narcoleptic.h>
#include <SoftwareSerial.h>
#include <WiFlyHQ.h>
#include "wifi_data.h"

float temp;

void terminal();
float readTemp();
void sendTemp();
void powerOnLed();
void powerOffLed();

SoftwareSerial wifiSerial(8,9);
WiFly wifly;

//Temp reading
int sensor_reading;
float ctemperature;
const int B=3975; 
float resistance;

const int ledPin = 13;


void setup(){
  char buf[64];
  pinMode(ledPin, HIGH);  

  powerOffLed();

  Serial.begin(115200);
  Serial.println("Starting");

  wifiSerial.begin(9600);
  if (!wifly.begin(&wifiSerial, &Serial)) {
    Serial.println("Failed to start wifly");
    terminal();
  }

  /* Join wifi network if not already associated */
  if (!wifly.isAssociated()) {
    /* Setup the WiFly to connect to a wifi network */
    Serial.println("Joining network");
    wifly.setSSID(mySSID);
    wifly.setPassphrase(myPassword);
    wifly.enableDHCP();

    if (wifly.join()) {
      Serial.println("Joined wifi network");
    } 
    else {
      Serial.println("Failed to join wifi network");
      terminal();
    }
  } 
  else {
    Serial.println("Already joined network");
  }

  powerOnLed();

  if (wifly.isConnected()) {
    Serial.println("Old connection active. Closing");
    wifly.close();
  }

}

void loop() {

  temp = readTemp();
  Serial.println(temp);

  sendTemp(temp);

  powerOnLed();
  Narcoleptic.delay(600000);
  powerOffLed();
}

float readTemp() {
  sensor_reading=analogRead(0);
  resistance=(float)(1023-sensor_reading)*10000/sensor_reading; 
  ctemperature=1/(log(resistance/10000)/B+1/298.15)-273.15;

  return ctemperature; 
}

void sendTemp(float temp) {
  if (wifly.open(site, port)) {
    Serial.print("Connected to ");
    Serial.println(site);

    /* Send the request */
    wifly.println("POST /sensor HTTP/1.1");
    wifly.println("Accept: application/json");
    wifly.println("Content-type: application/json");
    wifly.println("Connection: close");
    wifly.print("Content-Length: ");
    wifly.println(sizeof(temp));
    wifly.println("");
    wifly.println(temp);
    wifly.close();
  } 
  else {
    Serial.println("Failed to connect");
  }
    
}

void terminal()
{
  while (1) {
    if (wifly.available() > 0) {
      Serial.write(wifly.read());
    }


    if (Serial.available()) { // Outgoing data
      wifly.write(Serial.read());
    }
  }
}

void powerOffLed() {  
  digitalWrite(ledPin, LOW);  
}

void powerOnLed() {  
  digitalWrite(ledPin, HIGH);  
}
