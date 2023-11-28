#include <stdio.h>
#include <stdint.h>
#include "RootDir.h"
FILE *file;
DirectoryEntry EntryTemp;
DirectoryEntry* ArrayEntry;
Root_Status ReadEntry(uint32_t PtrAddress);
Root_Status CheckEntry(uint32_t PtrAddress);
uint8_t CountElement(uint32_t PtrAddress);

/* The function reads an entry at the position passed in */
Root_Status ReadEntry(uint32_t PtrAddress)
{
	Root_Status status = READ_FAILED;
    /* Move the file pointer to the Address of the file */
    fseek(file, PtrAddress, SEEK_SET);

    /* Read an entry at the file pointer location */
    if (fread(&EntryTemp,sizeof(EntryTemp),1,file) != 0)
    {
        status = READ_SUCCESS;
    }
    return status;
}

/* The function to check whether the entry is valid or not? */
Root_Status CheckEntry(uint32_t PtrAddress)
{
	uint8_t Name0=0; // first byte value of entry (first byte of name)
	uint8_t Attri=0; // attributes value of entry 
	Root_Status status = ENTRY_OK;
	
    /* Move the file pointer to the Address of the file */
    fseek(file, PtrAddress, SEEK_SET);
    Name0=fgetc(file);
    /* Move the file pointer to the attributes of the Entry */
    fseek(file, PtrAddress+0xB, SEEK_SET);
    Attri=fgetc(file);
    
    /* Check the end of the entry list, The first byte of name has value 0x00  */
    if(Name0==0x00) //check long file name
    {
    	status = LIST_END;
	}
	/* Check for hidden entries, names starting with "." or ".." */
	if(Name0==0x2E) 
    {
    	status = HIDDEN_ENTRY;
	}
	/* Check if the entry contains a long file name  */
	if(Attri==0x0F) 
	{
		status = LONG_FILE_NAME;
	}
		/* Check for deleted entries  */
	if(Name0==0xE5) 
	{
		status = ENTRY_DELETED;
	}
    return status;
}


/* Counts the number of file or folder elements at the given address */
uint8_t CountElement(uint32_t PtrAddress)
{
	Root_Status status = READ_SUCCESS;
	/* Declare the initial number of elements as 0 */
	uint8_t NumElement=0;
	while(status!=LIST_END) 
   {
    /* Check Entry */
    status = CheckEntry(PtrAddress);
	    if(status==ENTRY_OK)
	    {
	    	NumElement++; //Increase the counter variable by 1 when the entry is valid
		}
		PtrAddress+=32; //Moves the input address to the next entry location
		
	}
	return NumElement;
}

/* Creates an array of elements at the passed position */
DirectoryEntry* CreateArrayEntry(uint32_t PtrAddress)
{
	Root_Status status = READ_SUCCESS;
	/* Open file */
	file = fopen("floppy.img", "rb");
	uint8_t Index=0;
	/* Calculate the number of elements in the array */
	uint8_t Num_Array= CountElement(PtrAddress); 
	/* Allocate memory for an array with the number of elements Num_Array */
	if(NULL!=ArrayEntry)
	{
		free(ArrayEntry);
	};
	ArrayEntry=(DirectoryEntry*)malloc(Num_Array);
	printf("cucuc\n");
   while(status!=LIST_END)
   {
    /* Read the Entry */
    status = CheckEntry(PtrAddress); //Check if the entry is valid
	    if(status==ENTRY_OK)
	    {
	    	ReadEntry(PtrAddress);
	    	ArrayEntry[Index]=EntryTemp; //assign valid entries to the array
	    	Index++;
		}
		PtrAddress=PtrAddress+32; //Moves the cursor to the next entry
	}
	return ArrayEntry;
}



