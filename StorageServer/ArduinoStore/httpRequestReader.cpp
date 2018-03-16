#include "httpRequestReader.h"
#include <math.h>

/*
 * Read a whole line of request
 * that is read until CRLF is found
 */
//int httpRequestReadln(WiFiClient client, char *requestContent, int length) {
int httpRequestReadln(EthernetClient client, char *requestContent, int length) {
  bool isCr = false;
  byte index = 0;
  while (client.connected()) { //At least make sure there's something to read
    if (client.available()) {
      char ch = client.read();
      if (index >= length) index = length - 1; //limited space
      requestContent[index] = ch;
      requestContent[++index] = 0;
      //set the next index as '\0' and increment by 1
      if (ch == '\n' && isCr) {
          return 1; //Finished reading a line;
      }
      if (ch == '\r') isCr = true;
      else isCr = false;
    }
  }
  if (index == 0) return 0; //this means the client is disconnected or there's no data to read
 }

/*
 * clear char array
 * This is just a helper function for httpRequestReadln()
 * After reading a line, people might want to reset clientRequest, due to limited Arduino memory
 */
void clearStringArr(char* ch_arr) {
  int i = 0;
  while (*(ch_arr + i) != 0) {
    *(ch_arr+i) = 0;
    i++;
  }
}

/*
 * Get the file name in the Content-Disposition: header
 * If no file is found, set filename to NULL
 */
void getFileNameFromContentDisposition(const char* clientRequest, char *filename) {
    char *token = "filename=\"";
    int index = findIndexOf(clientRequest, 80, token, 10);
    int j = 0;
    bool cond = (index != -1);
    if (cond) {
        for (int i = index + 10; i < 80; i++) {
            char ch = clientRequest[i];
            printf("%c\n", ch);
            if (ch != '\"') {
                //keep reading
                filename[j++] = ch;
            } else {
                filename[j] = 0;
                return;
            }
        }
    } else {
        filename = NULL;
    }
}

int getFileLengthFromContentLength(const char* clientRequest) {
    byte i = 16;
    char ch = clientRequest[i];
    byte index = 0;
    byte temp[25];
    unsigned long fileLength = 0;
    while (ch != '\r') {
        temp[index++] = (byte)(ch - 48);
        ch = clientRequest[++i];
    }
    byte tempLength = i - 16;
    for (int j = 0; j < tempLength; j++) { 
        fileLength += ((int)(temp[j])) * intPow(8, tempLength - 1 - j);
    }
    return fileLength;
}

/*
 * Happends to see this extremely elegant method, so used it :)
 */
int intPow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        base *= base;
    }
    return result;
}

/*
  * a helper function for getFileNameFromContentDisposition()
  * Find the first occurrence of the first letter of the substring
  * return the index
  * If NOT found, return -1
  */
int findIndexOf(const char* someArr, int length1, char* substring, int length2) {
    for (int i = 0; i < length1; i++) {
        if (someArr[i] == substring[0]) {
            for (int j = 1; j < length2 && j + i < length1; j++) {
                if (someArr[j + i] != substring[j]) break;
                else {
                    if (j == length2 - 1) return i;
                }
            }
        }
    }
    return -1;
}

/*
   * Get the File Type, only support .txt, .css, .png, .gif, .pdf, .zip, .xml, .sql
   */
char* getFileType(char* filename, int n) {
    byte index = -1;
    //find the index of '.' first
    for (byte i = 0; i < n; i++) {
        if(filename[i] == '.') {
            index = i;
            break;
        }
    }
    char filePost[4];
    if (index == -1) return "text/plain";
    byte max = (n - index > 3) ? (3) : (n - index);
    memcpy(filePost, filename + index + 1, max);
    filePost[3] = 0;
    if(strncmp(filePost, "txt", 3) == 0) return "text/plain";
    else if (strncmp(filePost, "css", 3) == 0) return "text/css";
    else if (strncmp(filePost, "png", 3) == 0) return "image/png";
    else if (strncmp(filePost, "gif", 3) == 0) return "image/gif";
    else if (strncmp(filePost, "pdf", 3) == 0) return "application/pdf";
    else if (strncmp(filePost, "zip", 3) == 0) return "application/zip";
    else if (strncmp(filePost, "xml", 3) == 0) return "application/xml";
    else if (strncmp(filePost, "sql", 3) == 0) return "application/sql";
    else return "text/plain"; //If not found, return text/plain anyway
}

/*
    * Get user-agent header content
    * User agent has Maximum length of 10
    */
 char* getUserAgent(char* clientRequest, char* userAgent) {
    int index = 12;
    int max = (strlen(clientRequest)-index) <= 9 ? strlen(clientRequest)-index : 9; //leaving one as null char
    memcpy(userAgent, clientRequest + index, max);
    userAgent[9] = 0; // Setting the null character
    return userAgent;
}

