#ifndef HTTPREQUESTREADER_H
#define HTTPREQUESTREADER_H
//#include <ESP8266WiFi.h>
#include <Ethernet.h>

/*
 * Read a whole line of request
 * that is read until CRLF is found
 */
//int httpRequestReadln(WiFiClient client, char *requestContent, int length);
int httpRequestReadln(EthernetClient client, char *requestContent, int length);

/*
 * clear char array
 * This is just a helper function for httpRequestReadln()
 * After reading a line, people might want to reset clientRequest, due to limited Arduino memory
 */
void clearStringArr(char* ch_arr);

/*
 * Get the file name in the Content-Disposition: header
 * If no file is found, set filename to NULL
 */
 void getFileNameFromContentDisposition(const char* clientRequest, char *filename);

 int intPow(int base, int exp);

 /*
  * a helper function for getFileNameFromContentDisposition()
  * Find the first occurrence of the first letter of the substring
  * return the index
  * If NOT found, return -1
  */
 int findIndexOf(const char* someArr, int length1, char* substring, int length2);

/*
 * Get the file length in the Content-Length: header
 */
int getFileLengthFromContentLength(const char* clientRequest);

  /*
   * Get the File Type, only support .txt, .css, .png, .gif, .pdf, .zip, .xml, .sql
   */
   char* getFileType(char* filename, int n);


 /*
    * Get user-agent header content
    * User agent has Maximum length of 10
    */
    char* getUserAgent(char* clientRequest, char *userAgent);

#endif
