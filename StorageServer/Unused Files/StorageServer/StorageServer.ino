#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
#include <string.h>
//#include "SDStorage.H"
//#include "HttpResponses.H"

//MAC address of the arduino
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
  };

EthernetServer server(80); //The server will listen to the port 80
/*
struct FileInformation {
  char* filename;
  char* path;
  int fileSizeInBytes;
};*/

void setup() {
  Serial.begin(9600); //Initialize the baud rate
  while(!Serial){;} //Loop until Serial is open
  Serial.println("Begin Connection");
  if(!Ethernet.begin(mac)) {
    Serial.println("cannot connect to the server");
    exit(0); //Open the ethernet connection
  }
  server.begin(); //open connection
  Serial.print("The server is at:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  char clientRequest[100];
  int i = 0;
  if (client) { //if there is a client
    Serial.println("A client is connected");

    boolean currentLineIsBlank = false;
    while(client.connected()) { //while there's still unread data
      if (client.available()) {
        char ch = client.read(); //read a char
        if(i >= 100) {
          i = 99;
        }
        clientRequest[i++] = ch;
        clientRequest[i] = 0; //set the next element as a null char
        Serial.print("Client request read:");
        Serial.println(clientRequest);

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if(ch == '\n') { //end of first line of the request
           char *token = strtok(clientRequest, "/");
           Serial.print("The token is:");
           Serial.println(token);
          if (strstr(token, "GET /") != 0) { 
          //if (contains(clientRequest, 100, "GET /", 5)) {
            //GET request received
            char* token = strtok(NULL, " ");//Seperated by a space
            Serial.println(token);
            if (strstr(token, "file-list") != 0) {
            //if (contains(clientRequest, 100, "file list", 9)) {
            //  HttpGETResponseList(client);
              client.flush();
              client.stop();
            }
            else { //Technically, token now should be "download"
              //Get the file name is the expected http request content
              char *filename = token; //File name should be the token
              client.stop();
            }
          } 
          else if (strstr(token, "POST /") != 0) { 
            //POST request received
             
          //  postRequestProcessing(client);
            
            client.stop();
          }
          else { //haven't implemented, return the list anyway
           // HttpGETResponseList(client);
            client.flush();
            client.stop();
          }
        }
      }
    }
  }
}

void postRequestProcessing(EthernetClient client) {
  bool currentLineIsBlank = false;
  while (client.connected()) {
    char ch = client.read();
    if (ch == '\n' && currentLineIsBlank) {
      //Finished reading HTTP Request, Pass the job back to the main function.
      return;
    }
    if (ch == '\n') {
      // you're starting a new line
      currentLineIsBlank = true;
    } else if (ch != '\r') {
      // you've gotten a character on the current line
      currentLineIsBlank = false;
    }
  }
}

bool contains(char* string, int n1, char* sub_string, int n2) {
    int index = 1;
    for (int i = 0; i < n1; i++) {
        if (string[i] == sub_string[0]) {
            while (index < n2 && i + index < n1) {
                if (sub_string[index] == string[i + index]) {
                    index++;
                } else {
                    index = 1;
                    break;
                }
            }

            if (index == n2) {
                return true;
            }
        }
    }
    return false;
}


/*
String getFileList(String directory) {
  String fileList = "";
  while(true){
    File root = SD.open("/"); //Open the root directory
    File entry = root.openNextFile();
    if (! entry) {
      //Serial.println("there's no file in the directory");
      break;
    }
    fileList += entry.name() + ',';
  }
  return fileList;
}*/


