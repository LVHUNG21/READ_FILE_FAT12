/******************************************************************************
 * Include
 *****************************************************************************/
#ifndef _FILE_HANDLING_H
#define _FILE_HANDLING_H

#include <string.h>
#include <stdio.h>
#include <time.h>
#include "../../FAT_Parse/ReadEntry/RootDir.h"
#include "../../FAT_Parse/Parse_data/FAT_Parse.h"
#include "../../FAT_Parse/BootSector/bootSector.h"
#include "../../Display/io.h"
/******************************************************************************
 * Macro
 *****************************************************************************/
#define MAX_ROOT 142

typedef enum FILE_HANDLING_STATUS
{
    FILE_HAND_CREATE_FILE_OK = 0,
    FILE_HAND_CREATE_FOLDER_OK,
    FILE_HAND_DELETE_OK,
    FILE_HAND_CREATE_FILE_FAILD,
    FILE_HAND_CREATE_FOLDER_FAILD,
    FILE_HAND_DELETE_FAILD,
    FILE_HAND_DUPLICATE_NAME
} FILE_HANDLING_STATUS;

/******************************************************************************
 * APIs
 *****************************************************************************/
/**
 * @brief Creates a file object.
 *
 * @param[in] file A pointer to a FILE object.
 * @param[in] filename The name of the file.
 * @param[in] fileSize The size of the file.
 * @param[in] address The address of the file.
 * @return The status of the file handling operation as a FILE_HANDLING_STATUS value.
 */
FILE_HANDLING_STATUS createFile(FILE *file, char *filename, uint32_t fileSize, uint32_t address);

/**
 * @brief                Creates a folder object.
 *
 * @param[in] file       A pointer to a FILE object.
 * @param[in] foldername The name of the folder.
 * @param[in] address    The address of the folder.
 * @return               The status of the file handling operation as a FILE_HANDLING_STATUS value.
 */
FILE_HANDLING_STATUS createFolder(FILE *file, char *foldername, uint32_t address);

/**
 * @brief          Deletes a file.
 *
 * @param file     A pointer to a FILE object.
 * @param filename The name of the file.
 * @param address  The address of the file.
 * @param entry    The directory entry representing the file.
 * @return         The status of the file handling operation as a FILE_HANDLING_STATUS value.
 */
FILE_HANDLING_STATUS deleteFile(FILE *file, char *filename, uint32_t address, DirectoryEntry entry);

/**
 * @brief             Allocates clusters.
 *
 * @param file        A pointer to a FILE object.
 * @param numClusters The number of clusters to allocate.
 * @return            The address of the allocated cluster as a uint32_t value.
 */
uint32_t allocateCluster(FILE *file, uint32_t numClusters);

#endif
/******************************************************************************
 * EOF
 *****************************************************************************/