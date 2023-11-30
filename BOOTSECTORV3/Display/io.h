#ifndef _IO_H
#define _IO_H
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>
#include "../FAT_Parse/FileHandling/file_handling.h"
// #include "../FAT_Parse/ReadEntry/RootDir.h"
union DateTime
{
    struct
    {
        uint16_t day : 5;
        uint16_t month : 4;
        uint16_t year : 7;
        uint16_t minute : 6;
        uint16_t hour : 5;
    } fields;
    uint16_t packed;
};

union Time
{
    struct
    {
        uint16_t second : 5;
        uint16_t minute : 6;
        uint16_t hour : 5;
    } fields;
    uint16_t packed;
};
void updateCurrentPath(char *currentPath, char *formattedName);
void displayAppStarting();
void removeSpaces(char *str);
uint32_t printList(struct DirectoryEntry *entries, uint32_t *entryCont, uint32_t address, uint8_t check_null, char currenpath[100]);

void displayOpeningFolder(char *folderName, char *currentPath);

void displayOpeningFile(char *fileName, char *currentPath);

void displayInvalidChoice();
void displayCurrentPath(char *currentPath);

void getUserInputFilename(char *filename);

void getUserInputFileSize(uint32_t *fileSize);

void displayFolderDeletionPrompt();

void displayFolderDeletionMessage(char *folderName);

void getUserInputFoldername(char *foldername);
int getUserInputFolderIndex();
void loadingBar(char str[20], uint8_t type);
void displayFileContent(uint8_t *data, size_t dataSize);
uint8_t backOption();

// void displayStatusMessage(FILE_HANDLING_STATUS status);

void getDataFromUser(uint8_t *buffer);
#endif