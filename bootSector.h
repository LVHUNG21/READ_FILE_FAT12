#ifndef BOOTSECTOR_H
#define BOOTSECTOR_H
#include <stdlib.h>
#include <stdio.h>
// #include "RootDir.h"
#include <stdint.h>
extern FILE *file;

typedef enum
{
    READ_OK = 0,
    READ_FAILD,
    BOOT_CALCULATE_OK,
    BOOT_FAILED,
} BootSector_Status;


#define NUM_BS_jmpBoot 3
#define NUM_BS_OEMName 8
#define NUM_BPB_BytsPerSec 2
#define NUM_BPB_SecPerClus 1
#define NUM_BPB_RsvdSecCnt 2
#define NUM_BPB_NumFATs 1
#define NUM_BPB_RootEntCnt 2
#define NUM_BPB_TotSec16 2
#define NUM_BPB_Media 1
#define NUM_BPB_FATSz16 2
#define NUM_BPB_SecPerTrk 2
#define NUM_BPB_NumHeads 2
#define NUM_BPB_HiddSec 4
#define NUM_BPB_TotSec32 4
#define NUM_BS_DrvNum 1
#define NUM_BS_Reserved1 1
#define NUM_BS_BootSig 1
#define NUM_BS_VolID 4
#define NUM_BS_VolLab 11
#define NUM_BS_FilSysType 8
#define NUM_BootCode 448
#define NUM_BootSignature 2

struct BootSector
{
    uint8_t BS_jmpBoot[NUM_BS_jmpBoot];         /* 0x00-0x02: Jump instruction to boot code */
    uint8_t BS_OEMName[NUM_BS_OEMName];         /* 0x03-0x0A: Version/Operating System name */
    uint8_t BPB_BytsPerSec[NUM_BPB_BytsPerSec]; /* 0x0B-0x0C: Bytes per sector */
    uint8_t BPB_SecPerClus;                     /* 0x0D: Sectors per cluster */
    uint8_t BPB_RsvdSecCnt[NUM_BPB_RsvdSecCnt]; /* 0x0E-0x0F: Reserved sectors count (Sectors before FAT) */
    uint8_t BPB_NumFATs;                        /* 0x10: Number of FATs */
    uint8_t BPB_RootEntCnt[NUM_BPB_RootEntCnt]; /* 0x11-0x12: FAT12, FAT16: Number of entries in the Root Directory Table; FAT32: 0 */
    uint8_t BPB_TotSec16[NUM_BPB_TotSec16];     /* 0x13-0x14: FAT12, FAT16: Total sectors in the Volume; FAT32: 0 */
    uint8_t BPB_Media;                          /* 0x15: Volume Media descriptor byte */
    uint8_t BPB_FATSz16[NUM_BPB_FATSz16];       /* 0x16-0x17: FAT12, FAT16: Size of each FAT in sectors; FAT32: 0 (BPB_FATSz32) */
    uint8_t BPB_SecPerTrk[NUM_BPB_SecPerTrk];   /* 0x18-0x19: Sectors per track */
    uint8_t BPB_NumHeads[NUM_BPB_NumHeads];     /* 0x1A-0x1B: Number of heads */
    uint8_t BPB_HiddSec[NUM_BPB_HiddSec];       /* 0x1C-0x1F: Number of hidden sectors before the Volume */
    uint8_t BPB_TotSec32[NUM_BPB_TotSec32];     /* 0x20-0x23: 32-bit total sectors in the Volume */
    uint8_t BS_DrvNum;                          /* 0x24: Drive number */
    uint8_t BS_Reserved1[NUM_BS_Reserved1];     /* 0x25: Reserved (used by Windows NT) */
    uint8_t BS_BootSig;                         /* 0x26: Extended boot signature (0x29) */
    uint8_t BS_VolID[NUM_BS_VolID];             /* 0x27-0x2A: Volume serial number */
    uint8_t BS_VolLab[NUM_BS_VolLab];           /* 0x2B-0x35: Volume label */
    uint8_t BS_FilSysType[NUM_BS_FilSysType];   /* 0x36-0x3D: File system type (FAT12, FAT16, FAT) */
    uint8_t BootCode[NUM_BootCode];             /* 0x3E-0x1BD: The remainder of the bootstrap program */
    uint8_t BootSignature[NUM_BootSignature];   /* 0x1BE-0x1BF: Boot block 'signature' (0x55 followed by 0xAA) */
};

/**
 * @brief        Calculates the address of the root directory in the file system.
 *
 * @param[inout] rootDirAddress Pointer to store the calculated root directory address.
 * @return       BootSector_Status Status indicating the success or failure of the calculation.
 */
BootSector_Status calculateRootDirectoryAddress(uint32_t *rootDirAddress);

/**
 * @brief     Calculates the offset of data region in the file system.
 *
 * @param[in] None 
 * @return    uint32_t Offset of the data region.
 */
uint32_t dataoffset();

/**
 * @brief     Calculates the offset of a specific cluster in the file system.
 *
 * @param[in] ncluster Cluster number for which the offset is calculated.
 * @return    uint32_t Offset of the specified cluster.
 */
uint32_t clusteroffset(uint16_t ncluster);

/**
 * @brief                       Calculates the address of the Root Directory based on the information in the Boot Sector.
 *
 * @param[inout] rootDirAddress A pointer to store the calculated Root Directory address.
 * @return                      BootSector_Status An enum indicating the status of the calculation.
 */
BootSector_Status calculateRootDirectoryAddress(uint32_t *rootDirAddress);

/**
 * @brief                Reads the first Boot Sector from the FAT file system image.
 * @param[in]            None
 * @return               BootSector_Status An enum indicating the status of the read operation.
 */
BootSector_Status readBootSector();

/**
 * @brief                Calculates the address of the File Allocation Table (FAT) based on the information in the Boot Sector.
 *
 * @param[in] fatAddress A pointer to store the calculated FAT address.
 * @return               BootSector_Status An enum indicating the status of the calculation.
 */
BootSector_Status calculateFATAddress(uint32_t *fatAddress);

/**
 * @brief     Get the Byts Per Sec object
 *
 * @param[in] bootSector
 * @return    uint16_t
 */
uint16_t getBytsPerSec();

/**
 * @brief     Get the Sec Per Clus object
 *
 * @param[in] bootSector
 * @return    uint8_t
 */
uint8_t getSecPerClus();

/**
 * @brief Get the Rsvd Sec Cnt object
 *
 * @param bootSector
 * @return uint16_t
 */
uint16_t getRsvdSecCnt();

/**
 * @brief Get the Num F A Ts object
 *
 * @param bootSector
 * @return uint8_t
 */
uint8_t getNumFATs();

/**
 * @brief Get the Root Ent Cnt object
 *
 * @param bootSector
 * @return uint16_t
 */
uint16_t getRootEntCnt();

/**
 * @brief Get the Tot Sec16 object
 *
 * @param bootSector
 * @return uint32_t
 */
uint32_t getTotSec16();

/**
 * @brief Get the Media object
 *
 * @param bootSector
 * @return uint8_t
 */
uint8_t getMedia();

/**
 * @brief
 *
 * @param bootSector
 * @return uint16_t
 */
uint16_t getFATSz16();

/**
 * @brief Get the Sec Per Trk object
 *
 * @param bootSector
 * @return uint16_t
 */
uint16_t getSecPerTrk();

/**
 * @brief Get the Num Heads object
 *
 * @param bootSector
 * @return uint16_t
 */
uint16_t getNumHeads();

/**
 * @brief Get the Hidd Sec object
 *
 * @param bootSector
 * @return uint32_t
 */
uint32_t getHiddSec();

/**
 * @brief Get the Tot Sec32 object
 *
 * @param bootSector
 * @return uint32_t
 */
uint32_t getTotSec32();

/**
 * @brief Get the Drv Num object
 *
 * @param bootSector
 * @return uint8_t
 */
uint8_t getDrvNum();

/**
 * @brief Get the Boot Sig object
 *
 * @param bootSector
 * @return uint8_t
 */
uint8_t getBootSig();

/**
 * @brief Get the Vol I D object
 *
 * @param bootSector
 * @return uint32_t
 */
uint32_t getVolID();

#endif // BOOTSECTOR_H
