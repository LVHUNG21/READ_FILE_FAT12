#include <stdio.h>
#include <stdint.h>
#include "boot/bootSector.h"

FILE *file;

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

BootSector_Status calculateRootDirectoryAddress(uint32_t *rootDirAddress)
{
    file = fopen("floppy.img", "rb");
    struct BootSector bootSector;

    if (readBootSector(file, &bootSector) != READ_OK)
    {
        return READ_FAILD;
    }

    uint32_t reservedSectors = *((uint16_t *)(bootSector.BPB_RsvdSecCnt));
    uint32_t numFATs = bootSector.BPB_NumFATs;
    uint32_t fatsSize = *((uint16_t *)(bootSector.BPB_FATSz16));
    uint32_t bytesPerSector = *((uint16_t *)(bootSector.BPB_BytsPerSec));

    *rootDirAddress = (reservedSectors + numFATs * fatsSize) * bytesPerSector;

    return BOOT_CALCULATE_OK;
}

uint32_t dataoffset(struct BootSector *bs)
{
    uint32_t RootEntCnt = *(uint16_t *)bs->BPB_RootEntCnt;
    uint32_t rootAdd = 0;
    BootSector_Status status = calculateRootDirectoryAddress(&rootAdd);
    printf("statsub:%d", status);

    return 9728 + (RootEntCnt * 32);
}
uint32_t clusteroffset(struct BootSector *bs, uint16_t ncluster)
{
    uint16_t SecPerClus = getSecPerClus(bs);
    uint16_t BytePerSec = *(uint16_t *)bs->BPB_BytsPerSec;

    printf("calculate:%x %x %x\n", SecPerClus, BytePerSec, dataoffset(bs));
    return dataoffset(bs) + ((ncluster - 2) * SecPerClus * BytePerSec);
}

BootSector_Status readBootSector(FILE *file, struct BootSector *bootSector)
{
    fseek(file, 0, SEEK_SET);

    if (fread(bootSector, sizeof(struct BootSector), 1, file) != 1)
    {
        return READ_FAILD;
    }

    return READ_OK;
}

BootSector_Status calculateFATAddress(FILE *file, struct BootSector bootSector, uint32_t *fatAddress)
{
    uint32_t reservedSectors = *((uint16_t *)(bootSector.BPB_RsvdSecCnt));
    uint32_t bytesPerSector = *((uint16_t *)(bootSector.BPB_BytsPerSec));

    *fatAddress = reservedSectors * bytesPerSector;

    return BOOT_CALCULATE_OK;
}

uint16_t getBytsPerSec(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_BytsPerSec);
}

uint8_t getSecPerClus(struct BootSector *bootSector)
{
    return bootSector->BPB_SecPerClus;
}

uint16_t getRsvdSecCnt(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_RsvdSecCnt);
}

uint8_t getNumFATs(struct BootSector *bootSector)
{
    return bootSector->BPB_NumFATs;
}

uint16_t getRootEntCnt(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_RootEntCnt);
}

uint32_t getTotSec16(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_TotSec16);
};

uint8_t getMedia(struct BootSector *bootSector)
{
    return bootSector->BPB_Media;
};

uint16_t getFATSz16(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_FATSz16);
};

uint16_t getSecPerTrk(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_SecPerTrk);
};

uint16_t getNumHeads(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_NumHeads);
};

uint32_t getHiddSec(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_HiddSec);
};

uint32_t getTotSec32(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_TotSec32);
};

uint8_t getDrvNum(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BPB_DrvNum);
};

uint8_t getBootSig(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BootSignature);
};

uint32_t getVolID(struct BootSector *bootSector)
{
    return *((uint16_t *)bootSector->BS_VolID);
};