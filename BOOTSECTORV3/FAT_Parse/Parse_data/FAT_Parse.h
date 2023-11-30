#include<stdio.h>
#include "../../FAT_Parse/BootSector/bootSector.h"
#include "../../FAT_Parse/ReadEntry/RootDir.h"
// #include "../../Display/Display.h"
#ifndef _FAT_PARSE_H
#define _FAT_PARSE_H
// extern FILE* file;

void readContentAndDisplay(FILE *file, uint32_t startCluster, uint32_t fileSize,char path[100]);

uint32_t fatoffset(uint32_t nfat);

uint16_t fatentry(FILE *file, uint32_t ncluster);

void readContentFromClusters(FILE *file, uint32_t startCluster, uint32_t fileSize);

void loadingBar(char str[20],uint8_t type);

#endif
