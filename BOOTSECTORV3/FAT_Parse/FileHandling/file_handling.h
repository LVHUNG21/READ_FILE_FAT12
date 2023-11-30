
#ifndef _FILE_HANDLING_H
#define _FILE_HANDLING_H

#include <string.h>
#include <stdio.h>
#include <time.h>
#include "../../FAT_Parse/ReadEntry/RootDir.h"
#include "../../FAT_Parse/Parse_data/FAT_Parse.h"
#include "../../FAT_Parse/BootSector/bootSector.h" 
#include "../../Display/io.h"
// extern FILE *file;
#define MAX_ROOT 142 

typedef enum FILE_HANDLING_STATUS{
    FILE_HAND_CREATE_FILE_OK=0,
    FILE_HAND_CREATE_FOLDER_OK,
    FILE_HAND_DELETE_OK,
    FILE_HAND_CREATE_FILE_FAILD,
    FILE_HAND_CREATE_FOLDER_FAILD,
    FILE_HAND_DELETE_FAILD,
    FILE_HAND_DUPLICATE_NAME
}FILE_HANDLING_STATUS;

FILE_HANDLING_STATUS createFile(FILE *file,  char *filename, uint32_t fileSize,uint32_t address);

FILE_HANDLING_STATUS createFolder(FILE *file,  char *foldername,uint32_t address);

FILE_HANDLING_STATUS deleteFile(FILE *file,  char *filename, uint32_t address,DirectoryEntry entry);

uint32_t allocateCluster( FILE *file, uint32_t numClusters);



// void loadingBar(char str[20],uint8_t type);


#endif