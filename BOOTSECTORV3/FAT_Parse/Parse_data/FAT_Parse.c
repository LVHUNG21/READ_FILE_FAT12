/******************************************************************************
 * Include
 *****************************************************************************/
#include "FAT_Parse.h"

/******************************************************************************
 * Function
 *****************************************************************************/

uint16_t fatentry(FILE *file, uint32_t ncluster)
{
    uint32_t fatoffsett = fatoffset(0);

    fatoffsett += 3 * (ncluster / 2);

    fseek(file, fatoffsett, SEEK_SET);

    uint8_t buffer[3];
    size_t bytesRead = fread(buffer, 1, 3, file);

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

    // printf("v:%d ", v);
    return v;
}
/******************************************************************************
 * EOF
 *****************************************************************************/

