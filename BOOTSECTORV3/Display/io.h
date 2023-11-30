/******************************************************************************
 * Include
 *****************************************************************************/
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

/******************************************************************************
 * APIS
 *****************************************************************************/
/**
 * @brief update current path
 * 
 * @param currentPath 
 * @param formattedName 
 */
void updateCurrentPath(char *currentPath, char *formattedName);

/**
 * @brief display title app starting
 * 
 */
void displayAppStarting();

/**
 * @brief remove space in path
 * 
 * @param str 
 */
void removeSpaces(char *str);

/**
 * @brief display List entry from address
 * 
 * @param entries 
 * @param entryCont 
 * @param address 
 * @param check_null 
 * @param currenpath 
 * @return uint32_t 
 */
uint32_t printList(struct DirectoryEntry *entries, uint32_t *entryCont, uint32_t address, uint8_t check_null, char currenpath[100]);

/**
 * @brief display when open foldere
 * 
 * @param folderName 
 * @param currentPath 
 */
void displayOpeningFolder(char *folderName, char *currentPath);

/**
 * @brief dispaly opening file
 * 
 * @param fileName 
 * @param currentPath 
 */
void displayOpeningFile(char *fileName, char *currentPath);

/**
 * @brief display notify invaild choice
 * 
 */
void displayInvalidChoice();
/**
 * @brief display current path
 * 
 * @param currentPath 
 */
void displayCurrentPath(char *currentPath);

/**
 * @brief Get the User Input Filename object
 * 
 * @param filename 
 */
void getUserInputFilename(char *filename);

/**
 * @brief Get the User Input File Size object
 * 
 * @param fileSize 
 */
void getUserInputFileSize(uint32_t *fileSize);

/**
 * @brief display folder prompt 
 * 
 */
void displayFolderDeletionPrompt();

/**
 * @brief display message when delete folder
 * 
 * @param folderName 
 */
void displayFolderDeletionMessage(char *folderName);

/**
 * @brief Get the User Input Foldername object
 * 
 * @param foldername 
 */
void getUserInputFoldername(char *foldername);
/**
 * @brief Get the User Input Folder Index object
 * 
 * @return int 
 */
int getUserInputFolderIndex();
/**
 * @brief display loading bar 
 * 
 * @param str 
 * @param type 
 */
void loadingBar(char str[20], uint8_t type);

/**
 * @brief dispaly file content
 * 
 * @param data 
 * @param dataSize 
 */
void displayFileContent(uint8_t *data, uint32_t dataSize);

/**
 * @brief Get back option from uuser
 * 
 * @return uint8_t 
 */
uint8_t backOption();

/**
 * @brief Get the Data From User object
 * 
 * @param buffer 
 */
void getDataFromUser(uint8_t *buffer);
#endif