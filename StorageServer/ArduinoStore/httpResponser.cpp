#include "httpResponser.h"

//void httpPOSTResponse(WiFiClient client, char *clientRequest) {
void httpPOSTResponse(EthernetClient client, char *clientRequest) {
      char fileName[13]; //Since the format of the file is 8.3, making the maximum length of 13
      int fileLength = 0;
      char userAgent[10];
      clearStringArr(clientRequest);
      byte count = 0; //Count for how many LINE FEED
      while (count <= 1) {
              //Serial.println("it's here");
        httpRequestReadln(client, clientRequest, 80);
        if((int)clientRequest[0] == 13 && (int)clientRequest[1] == 10) count++;
        if(strncmp(clientRequest, "Content-Disposition:", 20) == 0) {
           getFileNameFromContentDisposition(clientRequest, fileName);
           if (fileName == NULL) {
            httpBadRequest(client, "no file");
            Serial.println(F("NO FILE"));
           }
        }
        if(strncmp(clientRequest, "Content-Length:", 15) == 0) {
          fileLength = getFileLengthFromContentLength(clientRequest);
        }
        if(strncmp(clientRequest, "User-Agent:", 11) == 0) {
          getUserAgent(clientRequest, userAgent);
        }
        clearStringArr(clientRequest);
      }
      //After the while loop, we should be at the line "\r\n"
      //Therefore, read one more line
      Serial.println(F("Start writing file"));
      unsigned long counter = 0;
      //This function will open an existing file at the beginning of this file or 
      //Create a new file with this fileName
      File newFile;
      if (userAgent == NULL) {
        httpBadRequest(client, "No User Agent");
        client.flush();
        client.stop();
        return;
      }
      if (SD.exists(userAgent)) {
        newFile = SD.open(strcat(userAgent, fileName), FILE_WRITE);
      } else {
        SD.mkdir(userAgent);
        newFile = SD.open(strcat(userAgent,fileName), FILE_WRITE);
      }
      //filelist.txt is a reservered name, user cannot have file using the same name
      File fileList = SD.open(strcat(userAgent, "filelist.txt"), FILE_WRITE); 
      fileList.write(fileName);
      fileList.write("\r\n"); //Save new filename to file list
      fileList.close();
      while(counter <= fileLength) {
        if (client.available()) {
          char ch = client.read();
          Serial.print(ch);
          newFile.write(ch);
        } else {
          httpBadRequest(client, "not enough file content");
          Serial.println(F("not enough file content"));
          Serial.println(counter);
          newFile.close();
          client.flush(); client.stop();
          Serial.flush();
          return;
        }
        counter++;
      }
      Serial.println(F("Finished writing file"));
      newFile.close();  
      client.println(F("HTTP/1.1 200 OK"));
      client.println(F("Connection: close")); 
      htmlContentSending(client, "Victory!");
      Serial.println("Sent");
      client.flush();
      client.stop();
}

 //void httpGetResponseSendFile(WiFiClient client, char *filename) {
   void httpGetResponseSendFile(EthernetClient client, char *filename, char* clientRequest) {
    byte count = 0;
    char user[10];
    File myFile;
    while (count <= 1) {
      httpRequestReadln(client, clientRequest, 80);
      if((int)clientRequest[0] == 13 && (int)clientRequest[1] == 10) count++;
      if(strncmp(clientRequest, "User-Agent:", 11) == 0) {
          getUserAgent(clientRequest, user);
          break;
        }
        clearStringArr(clientRequest);
    }
    if (user == NULL) {
      httpBadRequest(client, "No User Agent");
        client.flush();
        client.stop();
        return;
    }
    if (SD.exists(user)) {
        myFile = SD.open(strcat(user, filename), FILE_WRITE);
      } else {
        SD.mkdir(user);
        //Before myFile, create a new Filelist.txt, if fileName is filelist.txt, it will return this empty filelist.txt
        File fileList = SD.open(strcat(user,filename), FILE_WRITE);
        fileList.close();
        myFile = SD.open(strcat(user,filename), FILE_WRITE);
      }
  myFile = SD.open(filename, FILE_READ);
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Connection: close")); 
  client.print(F("Content-Disposition: filename= "));
  client.println(filename);
  client.print(F("Content-Type: "));
  client.println(getFileType(filename, strlen(filename)));
  client.print(F("Content-Length: "));
  client.println(myFile.size());
  client.println();
  
  char by = myFile.read();
  Serial.print(F("File sent as:"));
  
  while (by != -1) {
    client.write(by); //client.write(&by, siezeof(by));
    Serial.write(by);
    by = myFile.read();
  }
  
  Serial.println("File sent!");
  myFile.close();
  client.flush();
  client.stop();
 }

//void httpBadRequest(WiFiClient client, char* message) {
void httpBadRequest(EthernetClient client, char* message) {
  client.println(F("HTTP/1.1 400 BadRequest"));
  htmlContentSending(client, message);
  client.println();
  client.flush();
  client.stop();
}

//void http404NotFound(WiFiClient client) {
void http404NotFound(EthernetClient client) {
  client.println(F("HTTP/1.1 404 NotFound"));
  htmlContentSending(client, "Ohh! File not found");
  client.println();
  client.flush();
  client.stop();
}

//void htmlContentSending(WiFiClient client, char* message) {
void htmlContentSending(EthernetClient client, char* message) {
  client.println(F("Content-Type: text/html"));
  client.println(F("Connection: close")); 
  client.println();
  client.println(F("<!DOCTYPE HTML>"));
  client.println(F("<html>"));
  client.println(F("<title> Little Arduino Storage </title>"));
  client.println(message);
  client.println(F("<br />"));
  client.println(F("</html>"));
  client.println();
}





