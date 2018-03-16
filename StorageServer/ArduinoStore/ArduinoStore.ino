/*
 * Arduino Storage
 * Group 42 Project - Jerry Huang, Taegoen Lee, Hongru Xiang
 * SE-2022 University of Waterloo
 * 
 * This is a mini online storage device made by arduino and ethernet shield.
 * 
 * Hardware: Arduino UNO, Ethernet Shield, ESP8266 wifi module, Ethernet Cable
 * 
 * Software: C / Java
 * 
 * Ethernet shield cannot use sd card slot and ethernet at the same time, Therefore, one can
 * probably use I2C and have to arduinos working together. One for receiving and sending HTTP
 * requests and responses, the other for writing files. This will be implemented later, after
 * my busy final term period. :)
 * 
 * 2017-12-02
 */
 
#include <SPI.h>
//#include <ESP8266WiFi.h>
#include <Ethernet.h>
#include <SD.h>
#include <string.h>
#include "httpResponser.h"
#include "httpRequestReader.h"

//SdVolume volume;

//MAC address of the arduino
const byte PROGMEM mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

const char* mySsid = "SSID";
const char* myPassword = "PASSOWRD";

//WiFiServer server(80); //The server will listen to the port 80
EthernetServer server(80);
//IPAddress ip(10, 168, 1, 177);

void setup() {
  Serial.begin(9600); //Initialize the baud rate
  while(!Serial){;} //Loop until Serial is open
  Serial.println(F("Begin Connection!"));
   //Open the ethernet connection
   //WiFi.begin(mySsid, myPassword);
  if(!Ethernet.begin(mac)) {
    Serial.println(F("cannot connect to the server"));
    Serial.flush();
    exit(0);
  }
  server.begin(); //open connection
  Serial.print(F("link started! at:"));
//  Serial.println(WiFi.localIP()); 
  Serial.println(Ethernet.localIP());
  //Initialize SD 
/*
  if(!SD.begin(4, SPI_HALF_SPEED)) {
    Serial.println(F("SD failed :("));
    Serial.flush();
    exit(0);
  } 
  Serial.println(F("Initialized"));*/
}

void loop() {
  //WiFiClient client = server.available();
  EthernetClient client = server.available();
  char clientRequest[80];
  if (client){ //if there's a client, READ the first line of the request
    Serial.println(F("client connected"));
    if (httpRequestReadln(client, clientRequest, 80)){ //function implemented in httpRequestReader.h
      Serial.print(F("Client Request:"));
      Serial.println(clientRequest);
      if (strstr(clientRequest, "GET /")) {
        //Get Request Received
        Serial.println(F("GET Received!"));
        char *fileName = strtok(clientRequest, "/");
        fileName = strtok(NULL, " HTTP");
        //The fileName should be "\(filename or possibily empty) HTTP"
        // So, Here, we got the file name
        getFileName(fileName, sizeof(fileName) - 1);
        if(*fileName == '/') {
          httpBadRequest(client, "File Name not valid / or empty :)");
        } else {
          //Here fileName has to be 8.3 form, and no space is allowed
          //This will be ensured by the java end
          httpGetResponseSendFile(client, fileName, clientRequest);
        }
        
      }
      else if (strstr(clientRequest, "POST /")) {
        Serial.println(F("POST Received!"));
        httpPOSTResponse(client, clientRequest);      
      } else {
        httpBadRequest(client, "unspecified request");
      }
    }
  }
  //Ethernet.maintain(); //Allowing the renewal of DHCP leases, **POSSIBLY IP will change**
}

void getFileName(char* token, int length) {
  for (int i = 0; i < length - 1; i++) {
    if(token[i] == 32) {
      token[i] = 0;
      return;
    }
  }
 }

