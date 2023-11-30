#include "FAT_Parse.h"



uint16_t fatentry(FILE *file, uint32_t ncluster)
{
    uint32_t fatoffsett = fatoffset(0);
    printf("%d:ncluster", ncluster);

    fatoffsett += 3 * (ncluster / 2);
    printf("%d:fatoffse", fatoffsett);

    fseek(file, fatoffsett, SEEK_SET);

    uint8_t buffer[3];
    size_t bytesRead = fread(buffer, 1, 3, file);
    printf("b1%x %x %x ", buffer[0], buffer[1], buffer[2]);

    if (bytesRead != 3)
    {

        return 0xFFFF;
    }

    uint16_t v;

    if (ncluster % 2 == 0)
    {
        v = ((buffer[1] & 0x0F) << 8) | buffer[0];
    }
    else
    {
        v = ((buffer[2] << 4) & 0xFF0) | ((buffer[1] & 0xF0) >> 4);
    }

    printf("v:%d ", v);
    return v;
}

void readContentFromClusters(FILE *file, uint32_t startCluster, uint32_t fileSize)
{

    uint16_t SecPerClus = getSecPerClus();
    uint16_t BytePerSec = getBytsPerSec();
    size_t clusterSize = SecPerClus * BytePerSec;
    uint8_t *buffer = (uint8_t *)malloc(clusterSize);

    uint32_t currentCluster = startCluster;
    uint32_t offset = 0;

    while (currentCluster < 0xFF8 && fileSize > 0)
    {
        offset = clusteroffset(currentCluster);

        fseek(file, offset, SEEK_SET);
        size_t bytesRead = fread(buffer, 1, clusterSize, file);

        if (bytesRead > fileSize)
        {
            bytesRead = fileSize;
        }

        displayFileContent(buffer, bytesRead);

        fileSize -= bytesRead;
        offset += bytesRead;

        uint16_t fatEntry = fatentry(file, currentCluster);

        currentCluster = fatEntry;
    }

    free(buffer);
}
