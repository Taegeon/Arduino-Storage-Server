#ifndef HTTPRESPONSER_H
#define HTTPRESPONSER_H
//#include <ESP8266WiFi.h>
#include <Ethernet.h>
#include <SD.h>
#include "httpRequestReader.h"
#include <string.h>
//#include "SDStorage.h"

/*
 * Send HttpPostResponses
 * After that, it will close connection
 */
//void httpPOSTResponse(WiFiClient client, char* clientRequest);
void httpPOSTResponse(EthernetClient client, char* clientRequest);

/*Print Bad Http Request, when something went wrong, also with a message
 * After that, it will close connection
 */
void httpBadRequest(EthernetClient client, char* message);
//void httpBadRequest(WiFiClient client, char* message);

/*
 * Send 404 Not Found response if not file found
 */
//void http404NotFound(WiFiClient client);
void http404NotFound(EthernetClient client);
/*
 * Send the corresponding file
 */
 void httpGetResponseSendFile(EthernetClient client, char *filename, char* clientRequest);
  //void httpGetResponseSendFile(WiFiClient client, char *filename);

 /*
  * Html form file sending to client
  */
  void htmlContentSending(EthernetClient client, char* message);
  // void htmlContentSending(WiFiClient client, char* message);



 
#endif
