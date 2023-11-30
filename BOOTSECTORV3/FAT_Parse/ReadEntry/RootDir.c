#include <stdio.h>
#include <stdint.h>
#include "RootDir.h"
DirectoryEntry EntryTemp;
FILE *file;
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
	uint8_t Name0 = 0; // first byte value of entry (first byte of name)
	uint8_t Attri = 0; // attributes value of entry
	Root_Status status = ENTRY_OK;

	/* Move the file pointer to the Address of the file */
	fseek(file, PtrAddress, SEEK_SET);
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
DirectoryEntry *readEntriesFromFile(FILE *file, uint32_t offset, int *numEntries, uint8_t multi)
{
	DirectoryEntry *entries = NULL;
	DirectoryEntry entry;
	if (multi != 'y')
	{
		/* Move the file pointer to the specified offset */
		fseek(file, offset, SEEK_SET);

		/* Determine the size of each directory entry */

		/* Initialize the count of total entries to 0 */
		int totalEntries = 0;

		/* Read the first directory entry */

		fread(&entry, 32, 1, file);

		/* Count the total number of entries */
		while (entry.name[0] != 0x00)
		{
			/* Check conditions for a valid entry */
			if (entry.attributes != 0x0F &&
				(entry.name[0] != 0x2E) &&
				(entry.name[0] != 0xE5) &&
				(entry.name[0] != 0x2E && (entry.name[1] != 0x20 && (entry.name[1] != 0x2E || entry.name[2] != 0x20))))
			{
				totalEntries++;
			}

			/* Read the next directory entry */
			fread(&entry, 32, 1, file);
		}
		if (entries != NULL)
		{
			free(entries);
		}
		/* Allocate memory for the array of directory entries */
		entries = (DirectoryEntry *)malloc(totalEntries * sizeof(DirectoryEntry));

		/* Check if memory allocation was successful */
		if (entries != NULL && totalEntries != 0)
		{
			/* Reset the file pointer to the specified offset */
			fseek(file, offset, SEEK_SET);

			/* Initialize the current index for storing entries */
			int currentIndex = 0;

			/* Read the first directory entry again */
			fread(&entry, 32, 1, file);

			/* Store valid entries in the allocated array */
			while (entry.name[0] != 0x00)
			{
				/* Check conditions for a valid entry */
				if (entry.attributes != 0x0F &&
					(entry.name[0] != 0x2E) &&
					(entry.name[0] != 0xE5) &&
					(entry.name[0] != 0x2E && (entry.name[1] != 0x20 && (entry.name[1] != 0x2E || entry.name[2] != 0x20))))
				{
					entries[currentIndex] = entry;
					currentIndex++;
				}

				/* Read the next directory entry */
				fread(&entry, 32, 1, file);
			}

			/* Set the output parameter with the total number of entries */
			*numEntries = totalEntries;
		}
		else
		{
			entries = NULL;
		}
	}
	else
	{

		/* Initialize the count of total entries to 0 */
		int totalEntries = 0;

		uint32_t currentCluster = offset;

		while (currentCluster < 0xFF8)
		{
			fseek(file, clusteroffset(currentCluster), SEEK_SET);
			fread(&entry, 32, 1, file);

			while (entry.name[0] != 0x00)
			{
				if (entry.attributes != 0x0F &&
					(entry.name[0] != 0x2E) &&
					(entry.name[0] != 0xE5) &&
					(entry.name[0] != 0x2E && (entry.name[1] != 0x20 && (entry.name[1] != 0x2E || entry.name[2] != 0x20))))
				{
					totalEntries++;
				}

				/* Read the next directory entry */
				fread(&entry, 32, 1, file);
			}

			/* Move to the next cluster */
			currentCluster = fatentry(file, currentCluster);
		}

		if (entries != NULL)
		{
			entries = NULL;
		}
		/* Allocate memory for the array of directory entries */
		entries = (DirectoryEntry *)malloc(totalEntries * sizeof(DirectoryEntry));

		/* Check if memory allocation was successful */
		if (entries != NULL && totalEntries != 0)
		{
			uint32_t currentCluster = offset;
			int currentIndex = 0;

			while (currentCluster < 0xFF8)
			{
				fseek(file, clusteroffset(currentCluster), SEEK_SET);
				fread(&entry, 32, 1, file);

				while (entry.name[0] != 0x00)
				{
					if (entry.attributes != 0x0F &&
						(entry.name[0] != 0x2E) &&
						(entry.name[0] != 0xE5) &&
						(entry.name[0] != 0x2E && (entry.name[1] != 0x20 && (entry.name[1] != 0x2E || entry.name[2] != 0x20))))
					{
						entries[currentIndex] = entry;
						currentIndex++;
					}

					/* Read the next directory entry */
					fread(&entry, 32, 1, file);
				}

				/* Move to the next cluster */
				currentCluster = fatentry(file, currentCluster);
			}

			/* Set the output parameter with the total number of entries */
			*numEntries = totalEntries;
		}
		else
		{
			entries = NULL;
		}
	}

	/* Return the array of directory entries */
	return entries;
}
