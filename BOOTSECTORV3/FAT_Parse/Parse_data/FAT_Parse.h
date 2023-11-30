/******************************************************************************
 * Macro
 *****************************************************************************/
#ifndef _FAT_PARSE_H
#define _FAT_PARSE_H

/******************************************************************************
 * Include
 *****************************************************************************/
#include<stdio.h>
#include "../../FAT_Parse/BootSector/bootSector.h"
#include "../../FAT_Parse/ReadEntry/RootDir.h"

/**
 * @brief Calculates the value of the FAT offset.
 *
 * @param nfat The number of FATs.
 * @return The value of the FAT offset as a uint32_t.
 */
uint32_t fatoffset(uint32_t nfat);

/**
 * @brief          Calculates the next cluster.
 *
 * @param file     A pointer to a FILE object.
 * @param ncluster The current cluster.
 * @return         The next cluster as a uint16_t.
 */
uint16_t fatentry(FILE *file, uint32_t ncluster);

/**
 * @brief             Reads content from clusters.
 *
 * @param file         A pointer to a FILE object.
 * @param startCluster The starting cluster.
 * @param fileSize     The size of the file.
 */
void readContentFromClusters(FILE *file, uint32_t startCluster, uint32_t fileSize);


#endif
/******************************************************************************
 * EOF
 *****************************************************************************/
