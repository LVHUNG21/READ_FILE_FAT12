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
    // MEMORY_ALLOCATION_FAILED
    // DISPLAY_STATUS_
} DISPLAY_STATUS;

void readContentAndDisplay(FILE *file, uint32_t startCluster, uint32_t fileSize, char path[100]);

uint32_t fatoffset(uint32_t nfat);

void readContentFromClusters(FILE *file, uint32_t startCluster, uint32_t fileSize);

void fileSystemManager(FILE *file);

#endif
/******************************************************************************
 * EOF
 *****************************************************************************/