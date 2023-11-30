#ifndef _Display_H
#define _Display_H

/******************************************************************************
 * Include
 *****************************************************************************/
#include "../FAT_Parse/BootSector/bootSector.h"
#include "io.h"
#include "io2.h"
#include <string.h>
#include <stdio.h>
#include "../FAT_Parse/ReadEntry/RootDir.h"
#include "../FAT_Parse/FileHandling/file_handling.h"
#include "../FAT_Parse/Parse_data/FAT_Parse.h"
#include <windows.h>
#include <time.h>

extern FILE *file;
typedef struct ListNode
{
    uint32_t address;
    struct ListNode *next;
} ListNode;
typedef enum Diplaystatus
{
    DISPLAY_STATUS_PRINT_LIST_OK = 5,
    DISPLAY_STATUS_PRINT_DATA_OK,
} DISPLAY_STATUS;

/**
 * @brief              Reads file content and displays it.
 *
 * @param file         A pointer to a FILE object.
 * @param startCluster The starting cluster.
 * @param fileSize     The size of the file.
 * @param path         The path of the file.
 */
void readContentAndDisplay(FILE *file, uint32_t startCluster, uint32_t fileSize, char path[100]);

/**
 * @brief      Calculates the value of the FAT offset.
 *
 * @param nfat The number of FATs.
 * @return     The value of the FAT offset as a uint32_t.
 */
uint32_t fatoffset(uint32_t nfat);

/**
 * @brief              Reads content from clusters.
 *
 * @param file         A pointer to a FILE object.
 * @param startCluster The starting cluster.
 * @param fileSize     The size of the file.
 */
void readContentFromClusters(FILE *file, uint32_t startCluster, uint32_t fileSize);

/**
 * @brief      Manages the file system.
 *
 * @param file A pointer to a FILE object.
 */
void fileSystemManager(FILE *file);

#endif
/******************************************************************************
 * EOF
 *****************************************************************************/