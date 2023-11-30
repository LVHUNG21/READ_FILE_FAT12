#include "bootSectorInfoPrint.h" 
#include <stdio.h>

/**
 * @brief Prints information from the Boot Sector structure.
 *
 * @param bootSector A pointer to the Boot Sector structure.
 */
void printBootSectorInfo(struct BootSector *bootSector) {
    printf("Bytes per sector: %d\n", *(uint16_t *)(bootSector->BPB_BytsPerSec));
    printf("Sectors per cluster: %d\n", bootSector->BPB_SecPerClus);
    printf("Reserved sectors count: %d\n", *(uint16_t *)(bootSector->BPB_RsvdSecCnt));
    printf("Number of FATs: %d\n", bootSector->BPB_NumFATs);
    printf("Number of root directory entries: %d\n", *(uint16_t *)(bootSector->BPB_RootEntCnt));
    printf("Total sectors (16-bit): %d\n", *(uint16_t *)(bootSector->BPB_TotSec16));
    printf("Media descriptor byte: %d\n", bootSector->BPB_Media);
    printf("Size of each FAT (16-bit): %d\n", *(uint16_t *)(bootSector->BPB_FATSz16));
    printf("Sectors per track: %d\n", *(uint16_t *)(bootSector->BPB_SecPerTrk));
    printf("Number of heads: %d\n", *(uint16_t *)(bootSector->BPB_NumHeads));
    printf("Number of hidden sectors before partition: %d\n", *(uint32_t *)(bootSector->BPB_HiddSec));
    printf("Total sectors (32-bit): %d\n", *(uint32_t *)(bootSector->BPB_TotSec32));
}
