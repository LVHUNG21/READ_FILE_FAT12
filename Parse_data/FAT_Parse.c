#include "FAT_Parse.h"

void readContentAndDisplay(FILE *file, uint32_t startCluster, uint32_t fileSize, char path[100])
{
    uint8_t check = 'n';
    do
    {
        system("cls");
        // size_t clusterSize = 512;
        // unsigned char buffer[clusterSize];
        // printf("filesize:%x", fileSize);

        uint32_t offset = clusteroffset(startCluster);

        fseek(file, offset, SEEK_SET);
        if (1)
        {
            readContentFromClusters(file, startCluster, fileSize);
        }

        printf("Do you want to backc\n");
        scanf("%c", &check);

    } while (check != 'n');
    loadingBar(path, 'b');
}

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

printf("v:%d ",v);
    return v;

}

void displayFileContent(uint8_t *data, size_t dataSize)
{
    printf("Hexadecimal :%x\n", dataSize);

    for (size_t i = 0; i < dataSize; ++i)
    {
         printf("%02X ", data[i]);
    }
        printf("done");

     printf("\nASCII \n");

     for (size_t i = 0; i < dataSize; ++i)
     {

         char character = (char)data[i];

         if (character < 32 || character > 126)
         {

             printf(".");
         }
         else
         {
             printf("%c", character);
         }
     }

    printf("\n");
}


uint16_t read_fat_entry(FILE* fileptr, uint16_t cluster) 
{
	uint16_t value;
	printf("%d:ncl: ", 512 + (cluster + (cluster / 2)));
    fseek(fileptr, 512 + (cluster + (cluster / 2)), SEEK_SET);	// FAT_START is address where FAT begin(0x200)
    																	// in FAT12 system, each FAT entry using 12 bits(1.5 byte), so if start cluster is 8 
																		// we need to jump to element (8*1.5)th in FAT
    fread(&value, 1, 2, fileptr);

    if (cluster & 1) // if cluster is odd
	{
		printf("Value in bytes: %04X\n", value); //3 lines printf just to check

	    
        value = (value >> 4); // take 12 high bits
    } 
	else 			// if cluster is even
	{
		printf("Value in bytes: %04X\n", value);
	    
    	value = (value & 0xFFF); //take 12 low bits
    }

    printf("Value in decimal: %d\n\n", value);
    return value;
}



