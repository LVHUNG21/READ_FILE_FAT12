/******************************************************************************
 * Macro
 *****************************************************************************/
#ifndef ROOTDIR_H
#define ROOTDIR_H
/******************************************************************************
 * Include
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../Parse_data/FAT_Parse.h"


typedef enum
{
  ENTRY_OK = 0,
  READ_SUCCESS,
  READ_FAILED,
  ADD_OK,
  LIST_END,
  LONG_FILE_NAME,
  HIDDEN_ENTRY,
  CALCULATE_OK,
  MEMORY_ALLOCATION_FAILED,
  ENTRY_DELETED,
} Root_Status;

typedef struct __attribute__((packed)) DirectoryEntry
{
  uint8_t name[8];           /* Space padded filename ([0] = E5h if deleted, 05h if pending delete) */
  char ext[3];            /* File extension */
  uint8_t attributes;     /* File attributes bitfield */
  uint8_t reserved[10];   /* Reserved, system dependent uses */
  uint16_t created_time;  /* [15:11] = hours(0..23), [10:5] = minutes, [4:0] = seconds/2 */
  uint16_t created_date;  /* [15:9] = year - 1980, [8:5] = month, [4:0] = day */
  uint16_t start_cluster; /* Cluster where this directory entry starts */
  uint32_t file_size;     /* Size of the file in bytes */
} DirectoryEntry;

/**
 * @brief                Counts the number of elements.
 *
 * @param[in] file       A pointer to a FILE object.
 * @param[in] PtrAddress The address of the pointer.
 * @return               The count of elements as a uint8_t value.
 */
uint8_t CountElement(FILE* file, uint32_t PtrAddress);

/**
 * @brief                Creates an array entry.
 *
 * @param[in] file       A pointer to a FILE object.
 * @param[in] PtrAddress The address of the pointer.
 * @return               A pointer to a DirectoryEntry structure.
 */
struct DirectoryEntry *CreateArrayEntry(FILE* file, uint32_t PtrAddress);

/**
 * @brief Reads all entries from a file and stores them in an array.
 *
 * @param[in] file       A pointer to a FILE object.
 * @param[in] offset     The offset within the file to start reading entries.
 * @param[inout] numEntries A pointer to an integer to store the number of entries read.
 * @param[in] multi      A flag indicating if multiple entries should be read.
 * @return           A pointer to a DirectoryEntry structure representing the array of entries.
 */
struct DirectoryEntry *readEntriesFromFile(FILE *file, uint32_t offset, int *numEntries, uint8_t multi);

#endif // ROOTDIR_H
/******************************************************************************
 * EOF
 *****************************************************************************/