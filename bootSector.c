#include <stdio.h>
#include <stdint.h>
#include "bootSector.h"

// FILE *openFile(const char *filename, const char *mode)
// {
//     FILE *file = fopen(filename, mode);
//     if (file == NULL)
//     {
//         perror("Error opening file");
//         exit(EXIT_FAILURE);
//     }
//     return file;
// }
struct BootSector bs;

BootSector_Status calculateRootDirectoryAddress(uint32_t *rootDirAddress)
{
    file = fopen("floppy.img", "r+b");
    if (readBootSector() != READ_OK)
    {
        return READ_FAILD;
    }

    uint32_t reservedSectors = *((uint16_t *)(bs.BPB_RsvdSecCnt));
    uint32_t numFATs = bs.BPB_NumFATs;
    uint32_t fatsSize = *((uint16_t *)(bs.BPB_FATSz16));
    uint32_t bytesPerSector = *((uint16_t *)(bs.BPB_BytsPerSec));

    *rootDirAddress = (reservedSectors + numFATs * fatsSize) * bytesPerSector;

    return BOOT_CALCULATE_OK;
}

uint32_t dataoffset()
{
    uint32_t RootEntCnt = *(uint16_t *)bs.BPB_RootEntCnt;
    uint32_t rootAdd = 0;
    BootSector_Status status = calculateRootDirectoryAddress(&rootAdd);

    return 9728 + (RootEntCnt * 32);
}
uint32_t clusteroffset(uint16_t ncluster)
{
    uint16_t SecPerClus = getSecPerClus(bs);
    uint16_t BytePerSec = *(uint16_t *)bs.BPB_BytsPerSec;
    printf("calculate:%x %x %x\n", SecPerClus, BytePerSec, dataoffset());
    return 16896 + ((ncluster - 2) * SecPerClus * BytePerSec);
}

BootSector_Status readBootSector()
{
    fseek(file, 0, SEEK_SET);

    if (fread(&bs, sizeof(struct BootSector), 1, file) != 1)
    {
        return READ_FAILD;
    }

    return READ_OK;
}
BootSector_Status readBootSector2(struct BootSector *bs)
{
    fseek(file, 0, SEEK_SET);

    if (fread(bs, sizeof(struct BootSector), 1, file) != 1)
    {
        return READ_FAILD;
    }

    return READ_OK;
}
uint32_t fatoffset(uint32_t nfat)
{
    uint16_t bytesPerSector = *((uint16_t *)bs.BPB_BytsPerSec);
    uint16_t sectorsPerFat = *((uint16_t *)bs.BPB_FATSz16);
    uint16_t reservedSectors = *((uint16_t *)bs.BPB_RsvdSecCnt);
    return bytesPerSector * ((nfat * sectorsPerFat) + reservedSectors);
}

BootSector_Status calculateFATAddress(uint32_t *fatAddress)
{
    uint32_t reservedSectors = *((uint16_t *)(bs.BPB_RsvdSecCnt));
    uint32_t bytesPerSector = *((uint16_t *)(bs.BPB_BytsPerSec));

    *fatAddress = reservedSectors * bytesPerSector;

    return BOOT_CALCULATE_OK;
}

uint16_t getBytsPerSec()
{
    return *((uint16_t *)bs.BPB_BytsPerSec);
}

uint8_t getSecPerClus()
{
    return bs.BPB_SecPerClus;
}

uint16_t getRsvdSecCnt()
{
    return *((uint16_t *)bs.BPB_RsvdSecCnt);
}

uint8_t getNumFATs()
{
    return bs.BPB_NumFATs;
}

uint16_t getRootEntCnt()
{
    return *((uint16_t *)bs.BPB_RootEntCnt);
}

uint32_t getTotSec16()
{
    return *((uint16_t *)bs.BPB_TotSec16);
};

uint8_t getMedia()
{
    return bs.BPB_Media;
};

uint16_t getFATSz16()
{
    return *((uint16_t *)bs.BPB_FATSz16);
};

uint16_t getSecPerTrk()
{
    return *((uint16_t *)bs.BPB_SecPerTrk);
};

uint16_t getNumHeads()
{
    return *((uint16_t *)bs.BPB_NumHeads);
};

uint32_t getHiddSec()
{
    return *((uint16_t *)bs.BPB_HiddSec);
};

uint32_t getTotSec32()
{
    return *((uint16_t *)bs.BPB_TotSec32);
};

uint8_t getDrvNum()
{
    return bs.BS_DrvNum;
};

uint8_t getBootSig()
{
    return *((uint16_t *)bs.BootSignature);
};

uint32_t getVolID()
{
    return *((uint16_t *)bs.BS_VolID);
};