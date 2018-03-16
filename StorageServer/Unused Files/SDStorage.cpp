#include "SDStorage.h"

String sendFileList(EthernetClient client, String directory) {
  String fileList = ""; //try to expand this after
  while(true){
    File root = SD.open("/"); //Open the root directory
    if (root) {
     File entry = root.openNextFile();
      if (! entry) {
        //Serial.println("there's no file in the directory");
        root.rewindDirectory();
        break;
      }
      fileList += entry.name();
      fileList += ",";
    }
  }
  return fileList;
}

FileInformation getFileinfo(char *path, char* fileName) {
  File myFile = SD.open(path, FILE_READ);
  unsigned long fileSize = myFile.size(); //in bytes
  return FileInformation{fileName, path, fileSize};
}


/*File name can only be 8.3
 * 8 characters before the dot .
 * 3 characters after the .
 * return true or false
 */
boolean isFileNameValidSD(char* filename) {
  if (filename == '\0') return false;
  if (sizeof(filename) / sizeof (char) > 12) {return false;}
    char* token = strtok(filename, ".");
  if (token == NULL) { //empty filename is false
    return false;
  }
  if (sizeof(token) / sizeof (char) > 8) return false;
  token = strtok(NULL, ".");
  if (sizeof(token) / sizeof (char) > 3) return false;
  return true;
}


/*Save a given file, it will be moved to StorageServer.ino file later
 * return 1 for success, -1 for failure
 */
int saveFileSD(char* filename, char* path) {
  if (isFileNameValidSD(path)){
    path = strcat(path, filename);
    SD.open(path);
    
  } else {
    Serial.println("File name is not valid");
    return -1;
  }
}


int readFileSD(char* filename, char* path) {
  if (isFileNameValidSD(path)){
    path = strcpy(path, filename);
    SD.open(path);
    
  } else {
    return -1;
  }
}
