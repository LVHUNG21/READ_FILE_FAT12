#ifndef ROOTDIR_H
#define ROOTDIR_H
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

typedef struct DirectoryEntry
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

uint8_t CountElement(FILE* file,uint32_t PtrAddress);

struct DirectoryEntry *CreateArrayEntry(FILE*file,uint32_t PtrAddress);
struct DirectoryEntry *readEntriesFromFile(FILE *file, uint32_t offset, int *numEntries,uint8_t multi);

#endif // ROOTDIR_H
