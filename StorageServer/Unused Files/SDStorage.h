#ifndef SDSTORAGE_H
#define SDSTORAGE_H

#include <string.h>
#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>

typedef struct FileInformation {
  char *filename;
  char *path;
  unsigned long fileSize;
} FileInformation;

struct SDInfo{
  int totalVolumn;
  int remainingVolumn;
  int existingFiles;
};

/*Return the remaining volumn on the SD card */
int getRemainingVolumn();

/*Return the size of a given file */
int getFileSize(char* dir, char* filename);

/*Return a char[] array containing the filenames of the root directory
 * every filename is seperated by a comma ","
 */
String sendFileList(EthernetClient client, String directory);

boolean isFileNameValidSD(char* filename);

int saveFileSD(char* filename, char* path);

int readFileSD(char* filename, char* path);

FileInformation getFileinfo(char *path, char* fileName);

#endif

