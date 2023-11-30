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
 * @brief      calculate value fatoffset  
 * @param[in]  nfat 
 * @return     value fatofffset 
 */
uint32_t fatoffset(uint32_t nfat);

/**
 * @brief calculate next cluster
 * 
 * @param file 
 * @param ncluster 
 * @return uint16_t 
 */
uint16_t fatentry(FILE *file, uint32_t ncluster);

void readContentFromClusters(FILE *file, uint32_t startCluster, uint32_t fileSize);


#endif
/******************************************************************************
 * EOF
 *****************************************************************************/
