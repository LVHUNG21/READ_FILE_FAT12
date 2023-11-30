#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// Địa chỉ của bảng FAT12
#define FAT_START 512 * 1

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

#pragma pack(1)
typedef struct Directory_Entry {
  uint8_t  name[8];			/* Space padded filename ([0] = E5h if deleted, 05h if pending delete) */
  uint8_t  ext[3];			/* File extension */
  uint8_t  attributes;		/* File attributes bitfield */
  uint8_t  reserved[10];	/* Reserved, system dependent uses */
  uint16_t  time;			/* [15:11] = hours(0..23), [10:5] = minutes, [4:0] = seconds/2 */
  uint16_t  date;			/* [15:9] = year - 1980, [8:5] = month, [4:0] = day */
  uint16_t start_cluster;	/* Cluster where this directory entry starts */
  uint32_t file_size;		/* Size of the file in bytes */
} Root_Entry_t;

typedef enum {
	READ_OK,
	READ_FALSE
} Read_Entry_Status_t;

//void print_bytes(uint16_t value) 
//{
//    for (int i = sizeof(value) - 1; i >= 0; i--) 
//	{
//        uint8_t byte = (value >> (8 * i)) & 0xFF;
//        printf("%02X ", byte);
//    }
//}

// Function to read entry from FAT12 table
uint16_t read_fat_entry(FILE* fileptr, uint16_t cluster) 
{
	uint16_t value;
	
    fseek(fileptr, FAT_START + (cluster + (cluster / 2)), SEEK_SET);	// FAT_START is address where FAT begin(0x200)
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

// Function to find next cluster
uint16_t find_next_cluster(FILE* fileptr, uint16_t current_cluster) 
{
    uint16_t fat_entry = read_fat_entry(fileptr, current_cluster);

    if (fat_entry >= 0xFF8 && fat_entry <= 0xFFF) // last cluster in file
	{
        fat_entry = 0xFFFF;  // Value to show end of file
    } 

    return fat_entry;
 

}

// Function to convert cluster to address
uint32_t  Cluster_to_Add(uint16_t cluster)
{
	return (cluster + 0x1F) * 0x200;	
}

// Function to jump to address of cluster
void jump(FILE * fileptr, uint16_t cluster)
{
	fseek(fileptr, ((cluster + 0x1F) * 0x200), SEEK_SET);
}


// Function to print cluster
void print_cluster(FILE* fileptr, uint16_t start_cluster)
{
    uint16_t next_cluster = start_cluster;

    do {
        printf("Current Cluster: %d\n", next_cluster);
        uint16_t address = Cluster_to_Add(next_cluster);
        printf("Current Address: 0x%X\n", address);
        next_cluster = find_next_cluster(fileptr, next_cluster);
    } while (next_cluster != 0xFFFF);
}

// Function to print Time and date
void printTimeDate(uint16_t time, uint16_t date) 
{
    // Parse data of time
    uint16_t hours = (time >> 11) & 0x1F;
    uint16_t minutes = (time >> 5) & 0x3F;
    uint16_t seconds = (time & 0x1F) * 2;

    // Parse data of date
    uint16_t year = ((date >> 9) & 0x7F) + 1980;
    uint16_t month = (date >> 5) & 0xF;
    uint16_t day = date & 0x1F;

    // print
    printf("Time: %02d:%02d:%02d\n", hours, minutes, seconds);
    printf("Date: %04d-%02d-%02d\n", year, month, day);
}

int main(int argc, char *argv[]) {
	
	char buffer[33];
	uint16_t test;
	Read_Entry_Status_t status;
	FILE * ptr = fopen("floppy.img", "rb");

	
	fseek(ptr, 0x36, SEEK_SET);	//FAT12
	fread(buffer, 1, 5, ptr);
	printf("%s\n", buffer);

	
	
//	Root_Entry_t RootEntry[16];
//	status = Read_Entry(RootEntry, ptr);
//	printf("%d\n", status);
	
//	for (int i = 0; RootEntry[i].name[0] != 0; i++) {
//        // Combine name and extension to get the full file name
//        char full_name[13];
//        snprintf(full_name, sizeof(full_name), "%.8s.%.3s", RootEntry[i].name, RootEntry[i].ext);
//
//        printf("File %d: %s\n", i + 1, full_name);
//    }
	

	print_cluster(ptr, 1262);
	
	fseek(ptr, 19968, SEEK_SET);
	fread(&test, 1, 2, ptr);

	
	
	
	return 0;
}
