#include <stdio.h>
#include <stdint.h>
#include "RootDir.h"
DirectoryEntry EntryTemp;
struct DirectoryEntry *entries;
DirectoryEntry *ArrayEntry;
Root_Status ReadEntry(uint32_t PtrAddress);
Root_Status CheckEntry(uint32_t PtrAddress);

/* The function reads an entry at the position passed in */
Root_Status ReadEntry(uint32_t PtrAddress)
{
	Root_Status status = READ_FAILED;
	/* Move the file pointer to the Address of the file */
	fseek(file, PtrAddress, SEEK_SET);

	/* Read an entry at the file pointer location */
	if (fread(&EntryTemp, sizeof(EntryTemp), 1, file) != 0)
	{
		status = READ_SUCCESS;
	}
	return status;
}

/* The function to check whether the entry is valid or not? */
Root_Status CheckEntry(uint32_t PtrAddress)
{
	// file=fopen("floppy.img","rb");
	uint8_t Name0 = 0; // first byte value of entry (first byte of name)
	uint8_t Attri = 0; // attributes value of entry
	Root_Status status = ENTRY_OK;

	/* Move the file pointer to the Address of the file */
	fseek(file, PtrAddress, SEEK_SET);
	printf("ftell:%d", ftell(file));
	Name0 = fgetc(file);
	/* Move the file pointer to the attributes of the Entry */
	fseek(file, PtrAddress + 0xB, SEEK_SET);
	Attri = fgetc(file);

	/* Check the end of the entry list, The first byte of name has value 0x00  */
	if (Name0 == 0x00) // check long file name
	{
		status = LIST_END;
	}
	/* Check for hidden entries, names starting with "." or ".." */
	if (Name0 == 0x2E)
	{
		status = HIDDEN_ENTRY;
	}
	/* Check if the entry contains a long file name  */
	if (Attri == 0x0F)
	{
		status = LONG_FILE_NAME;
	}
	if (Name0 == 0xE5)
	{
		status = ENTRY_DELETED;
	}

	return status;
}

/* Counts the number of file or folder elements at the given address */
uint8_t CountElement(FILE *file, uint32_t PtrAddress)
{
	Root_Status status = READ_SUCCESS;
	/* Declare the initial number of elements as 0 */
	uint8_t NumElement = 0;
	while (status != LIST_END)
	{
		/* Check Entry */
		status = CheckEntry(PtrAddress);
		if (status == ENTRY_OK)
		{
			NumElement++; // Increase the counter variable by 1 when the entry is valid
		}
		PtrAddress += 32; // Moves the input address to the next entry location
	}
	return NumElement;
}

/* Creates an array of elements at the passed position */
DirectoryEntry *CreateArrayEntry(FILE *file, uint32_t PtrAddress)
{
	Root_Status status = READ_SUCCESS;
	/* Open file */
	// file = fopen("floppy.img", "r+b");
	uint8_t Index = 0;
	uint8_t check_null = 'y';
	/* Calculate the number of elements in the array */
	uint8_t Num_Array = CountElement(file, PtrAddress);
	/* Allocate memory for an array with the number of elements Num_Array */
	ArrayEntry = (DirectoryEntry *)malloc(Num_Array * sizeof(DirectoryEntry));

	while (status != LIST_END)
	{
		/* Read the Entry */
		status = CheckEntry(PtrAddress); // Check if the entry is valid
		if (status == ENTRY_OK)
		{
			ReadEntry(PtrAddress);
			ArrayEntry[Index] = EntryTemp; // assign valid entries to the array
			Index++;
			check_null = 'n';
		}
		PtrAddress = PtrAddress + 32; // Moves the cursor to the next entry
	}
	if (check_null == 'y')
	{
		ArrayEntry = NULL;
	}
	return ArrayEntry;
}

struct DirectoryEntry *readEntriesFromFile(FILE *file, uint32_t offset, int *numEntries)
{
	fseek(file, offset, SEEK_SET);

	int entrySize = sizeof(struct DirectoryEntry);

	int totalEntries = 0;
	while (1)
	{
		struct DirectoryEntry entry;
		fread(&entry, entrySize, 1, file);

		if (entry.name[0] == 0x00)
		{
			break;
		}
		if (entry.attributes == 0x0F)
		{
			continue;
		}
		if ((entry.name[0] == 0x2E) || (entry.name[0]==0xe5))
		{
			continue;
		}
		if(entry.name[0] == 0xE5)
		{
			continue;
		}
		totalEntries++;
	}
	if(NULL!=entries)
	{
		free(entries);
	}
	entries = (struct DirectoryEntry *)malloc(totalEntries * sizeof(struct DirectoryEntry));
	if (entries == NULL || totalEntries ==0)
	{
		entries=NULL;
		return entries;
	}

	fseek(file, offset, SEEK_SET);
	int currentIndex = 0;
	while (1)
	{
		struct DirectoryEntry entry;
		fread(&entry, entrySize, 1, file);

		if (entry.name[0] == 0x00)
		{
			break;
		}
		if (entry.attributes == 0x0F)
		{
			continue;
		}
		if ((entry.name[0] == 0x2E && (entry.name[1] == 0x20 || (entry.name[1] == 0x2E && entry.name[2] == 0x20))))
		{
			continue;
		}
		printf("entry.name%x ",entry.name[0]);
		if ((entry.name[0] == 0xE5))
		{
			continue;
		}

		entries[currentIndex] = entry;
		currentIndex++;
	}

	*numEntries = totalEntries;
	return entries;
}
