#include "file_handling.h"

void updateFAT(FILE *file, uint32_t cluster, uint16_t value);
int hasEnoughSpaceForEntry(FILE *file, uint32_t numEntriesm, uint32_t address);
void addEntriesToRootDirectory(FILE *file, DirectoryEntry *newEntry, uint32_t numEntries);
void getCurrentDateTime(uint16_t *date, uint16_t *time1)
{
    /* Get the current time in seconds since the epoch */
    time_t currentTime = time(NULL);

    /* Convert the current time to local time */
    struct tm *currentLocalTime = localtime(&currentTime);

    /* Calculate the date value */
    *date = ((currentLocalTime->tm_year - 80) << 9) | ((currentLocalTime->tm_mon + 1) << 5) | currentLocalTime->tm_mday;

    /* Calculate the time value */
    *time1 = (currentLocalTime->tm_hour << 11) | (currentLocalTime->tm_min << 5) | (currentLocalTime->tm_sec / 2);
}

FILE_HANDLING_STATUS DuplicateName(DirectoryEntry *entries, int numEntries, char *newName);

int isDuplicateName(DirectoryEntry *entries, int numEntries, char *newName)
{
    /* Get the length of the new name */
    int newnameLength = strlen(newName);

    /* Iterate through the directory entries */
    for (int i = 0; i < numEntries; ++i)
    {
        /* Get the length of the current entry's name */
        int entrynameLength = strlen(entries[i].name);

        /* If the lengths are different, skip to the next entry */
        if (entrynameLength != newnameLength)
        {
            continue;
        }

        /* Flag to track if the names match */
        int isMatch = 1;

        /* Compare each character of the names */
        for (int j = 0; j < newnameLength; ++j)
        {
            /* If a character doesn't match, set the flag to false and break the loop */
            if (entries[i].name[j] != newName[j])
            {
                isMatch = 0;
                break;
            }
        }

        /* If the names match, return 1 */
        if (isMatch)
        {
            return 1;
        }
    }

    /* If no duplicate name is found, return 0 */
    return 0;
}

enum FILE_HANDLING_STATUS deleteFile(FILE *file, char *filename, uint32_t address, DirectoryEntry entry)
{
    /* Set the file pointer to the specified address */
    fseek(file, address, SEEK_SET);

    DirectoryEntry entry1;
    uint8_t check = 'y';
    FILE_HANDLING_STATUS status = FILE_HAND_DELETE_FAILD;

    /* Read each entry from the file */
    while (fread(&entry1, sizeof(DirectoryEntry), 1, file) == 1)
    {
        /* Compare the name characters of the current entry with the specified entry */
        for (uint32_t i = 0; i < 8; i++)
        {
            if (entry1.name[i] != entry.name[i])
            {
                check = 'n';
                break;
            }
        }

        /* Compare the extension characters of the current entry with the specified entry */
        for (uint32_t i = 0; i < 3; i++)
        {
            if (entry1.ext[i] != entry.ext[i])
            {
                check = 'n';
                break;
            }
        }

        /* If all the attributes and values match, delete the entry */
        if (check == 'y' &&
            entry1.attributes == entry.attributes &&
            entry1.created_time == entry.created_time &&
            entry1.created_date == entry.created_date &&
            entry1.start_cluster == entry.start_cluster &&
            entry1.file_size == entry.file_size)
        {
            /* Set the file pointer to the beginning of the entry and mark it as deleted */
            fseek(file, ftell(file) - 32, SEEK_SET);
            uint8_t delete = 0xE5;
            fwrite(&delete, 1, 1, file);

            uint32_t startCluster = entry.start_cluster;

            /* Update the FAT entries to free the clusters allocated to the file */
            while (startCluster < 0xFF8)
            {
                uint16_t fatEntry = fatentry(file, startCluster);
                updateFAT(file, startCluster, 0x000);
                startCluster = fatEntry;
            }

            /* Set the status to indicate successful deletion */
            status = FILE_HAND_DELETE_OK;
        }

        /* Reset the check flag for the next entry */
        check = 'y';
    }

    /* Return the status of the file deletion operation */
    return status;
}

int hasEnoughSpaceForEntry(FILE *file, uint32_t numEntries, uint32_t rootOffset)
{
    /* Set the current offset to the root directory offset */
    uint32_t currentOffset = rootOffset;

    /* Adjust the number of entries based on the root offset */
    uint32_t root = 0;
    calculateRootDirectoryAddress(&root);

    numEntries = (rootOffset == root) ? numEntries : 15;

    /* Initialize a counter to track the number of occupied entries */
    uint32_t count = 0;

    DirectoryEntry entry;
    int status = 0;

    /* Iterate through the directory entries */
    for (uint32_t i = 0; i < numEntries; ++i)
    {
        /* Set the file pointer to the current offset */
        fseek(file, currentOffset, SEEK_SET);

        /* Read the directory entry */
        fread(&entry, sizeof(DirectoryEntry), 1, file);

        /* Check if the entry is occupied */
        if (entry.name[0] != 0x00 && entry.name[0] != 0xE5)
        {
            currentOffset += 32;
            count++;
        }
        else
        {
            count++;
        }

        /* Move to the next entry */
        currentOffset += 32;
    }

    /* If all entries are occupied, set the status to indicate insufficient space */
    if (count == numEntries)
    {
        status = 1;
    }

    /* Return the status of the space availability */
    return status;
}

FILE_HANDLING_STATUS createFile(FILE *file, char *filename, uint32_t fileSize, uint32_t address)
{
    /* Initialize the status variable */
    FILE_HANDLING_STATUS status = FILE_HAND_CREATE_FILE_OK;

    /* Create a new file entry */
    DirectoryEntry newFileEntry;

    /* Get the current date and time */
    uint16_t currentDate, currentTime;
    getCurrentDateTime(&currentDate, &currentTime);

    /* Get the number of sectors per cluster and bytes per sector */
    uint16_t SecPerClus = getSecPerClus();
    uint16_t BytePerSec = getBytsPerSec();

    /* Calculate the number of clusters needed for the file */
    uint32_t numClusters;
    if (fileSize < 512)
    {
        numClusters = 1;
    }
    else
    {
        numClusters = fileSize / (SecPerClus * BytePerSec);
        numClusters = numClusters + 1;
    }

    /* Read the directory entries from the file */
    uint32_t numEntries = 0;
    DirectoryEntry *listEntry = readEntriesFromFile(file, address, &numEntries, 'n');

    /* Check for duplicate name */
    if (isDuplicateName(listEntry, numEntries, filename))
    {
        status = FILE_HAND_DUPLICATE_NAME;
    }
    else
    {
        /* Initialize the new file entry */
        memset(&newFileEntry, 0, 32);
        strncpy(newFileEntry.name, filename, 8);
        newFileEntry.attributes = 0x20;
        strncpy(newFileEntry.ext, "TXT", 3);
        newFileEntry.created_date = currentDate;
        newFileEntry.created_time = currentTime;
        newFileEntry.file_size = fileSize;

        /* Set the root offset to the specified address */
        uint32_t rootOffset = address;
        fseek(file, rootOffset, SEEK_SET);

        /* Check if there is enough space for the new entry in the root directory */
        if (hasEnoughSpaceForEntry(file, MAX_ROOT, rootOffset) == 0)
        {
            status = FILE_HAND_CREATE_FILE_FAILD;
        }
        else
        {
            /* Allocate clusters for the file */
            uint32_t startCluster = allocateCluster(file, numClusters);

            /* Check if cluster allocation failed */
            if (startCluster == 0)
            {
                status = FILE_HAND_CREATE_FILE_FAILD;
            }
            else
            {
                /* Link the allocated clusters in the FAT */
                uint32_t currentCluster = startCluster;
                for (int i = 0; i < numClusters - 1; ++i)
                {
                    uint32_t nextCluster = allocateCluster(file, 1);
                    if (nextCluster != 1)
                    {
                        updateFAT(file, currentCluster, nextCluster);
                    }
                    currentCluster = nextCluster;
                }
                fseek(file, rootOffset, SEEK_SET);

                /* Set the start cluster of the new file entry */
                newFileEntry.start_cluster = startCluster;

                /* Add the new file entry to the root directory */
                addEntriesToRootDirectory(file, &newFileEntry, 1);

                /* get file content from user */
                uint8_t *buffer = (uint8_t *)malloc(512);

                getDataFromUser(buffer);
                /* Write the file content to the allocated clusters */
                uint32_t len = strlen(buffer);
                uint32_t offset = clusteroffset(currentCluster);
                fseek(file, offset, SEEK_SET);
                fwrite(buffer, 1, len, file);
            }
        }
    }

    /* Return the status of the file creation operation */
    return status;
}

/* Add new directory entries to the root directory of a file */
void addEntriesToRootDirectory(FILE *file, DirectoryEntry *newEntry, uint32_t numEntries)
{
    uint32_t rootOffset = 0;
    /* Function to calculate the root directory address */
    calculateRootDirectoryAddress(&rootOffset);

    uint32_t currentOffset = rootOffset;
    DirectoryEntry entry;

    /* Search for an empty entry in the root directory */
    while (fread(&entry, sizeof(DirectoryEntry), 1, file) == 1)
    {
        if (entry.name[0] == 0x00 || entry.name[0] == 0xE5)
        {
            currentOffset = ftell(file);
            break;
        }
    }

    fseek(file, currentOffset - 32, SEEK_SET);
    fwrite(newEntry, sizeof(DirectoryEntry), 1, file);
}

/* Create a new folder in the file system */
FILE_HANDLING_STATUS createFolder(FILE *file, char *foldername, uint32_t address)
{
    FILE_HANDLING_STATUS status = FILE_HAND_CREATE_FOLDER_OK;
    DirectoryEntry newFolderEntry;
    memset(&newFolderEntry, 0, sizeof(DirectoryEntry));
    strncpy(newFolderEntry.name, foldername, 8);
    newFolderEntry.attributes = 0x10;
    uint16_t currentDate, currentTime;
    /* Function to get the current date and time */
    getCurrentDateTime(&currentDate, &currentTime);
    newFolderEntry.created_date = currentDate;
    newFolderEntry.created_time = currentTime;

    uint32_t rootOffset = address;
    fseek(file, rootOffset, SEEK_SET);

    uint32_t currentOffset = rootOffset;
    DirectoryEntry entry;
    DirectoryEntry dotEntry;
    DirectoryEntry dotDotEntry;
    uint32_t exit = 0;
      uint32_t root = 0;
    calculateRootDirectoryAddress(&root);

    /* Iterate through the entries in the root directory */
    while (fread(&entry, 32, 1, file) == 1 && exit == 0)
    {
        if (entry.name[0] == 0x00 || entry.name[0] == 0xE5)
        {
            /* Function to allocate a cluster for the new folder */
            uint32_t firstCluster = allocateCluster(file, 1);

            if (firstCluster == 0)
            {
                status = FILE_HAND_CREATE_FOLDER_FAILD;
                exit = 1;
            }
            else
            {
                newFolderEntry.start_cluster = firstCluster;
                /* Function to update the file allocation table (FAT) */
                updateFAT(file, firstCluster, 0xFFF);


                if (address != root)
                {
                    DirectoryEntry dotEntry;
                    memset(&dotEntry, 0, sizeof(DirectoryEntry));
                    strncpy(dotEntry.name, ".", 8);
                    dotEntry.attributes = 0x10;
                    dotEntry.start_cluster = firstCluster;

                    DirectoryEntry dotDotEntry;
                    memset(&dotDotEntry, 0, sizeof(DirectoryEntry));
                    strncpy(dotDotEntry.name, "..", 8);
                    dotDotEntry.attributes = 0x10;
                    dotDotEntry.start_cluster = currentOffset;

                    fseek(file, currentOffset - 32, SEEK_SET);
                    fwrite(&dotEntry, sizeof(DirectoryEntry), 1, file);
                    fwrite(&dotDotEntry, sizeof(DirectoryEntry), 1, file);
                }

                fseek(file, currentOffset, SEEK_SET);
                fwrite(&newFolderEntry, sizeof(DirectoryEntry), 1, file);
                exit = 1;
            }
        }

        currentOffset += 32;
    }

    return status;
}

uint32_t allocateCluster(FILE *file, uint32_t numClusters)
{
    /* Initialize variables to track current and last clusters */
    uint32_t currentCluster = 0;
    uint32_t lastCluster = 0;

    /* Iterate through the FAT entries */
    for (uint32_t i = 0; i < 4600; ++i)
    {
        /* Read the FAT entry for the current cluster */
        uint16_t fatEntry = fatentry(file, i);

        /* Check if the FAT entry is empty */
        if (fatEntry == 0x000)
        {

            /* Check if it's the first empty cluster encountered */
            if (currentCluster == 0)
            {
                currentCluster = i;
            }
            lastCluster = lastCluster + 1;

            /* Check if enough consecutive free clusters are found */
            if (lastCluster == numClusters)
            {
                /* Calculate the cluster number of the last cluster in the chain */
                uint32_t lastClusterInChain = currentCluster + numClusters - 1;

                /* Update the FAT entries for the allocated clusters */
                updateFAT(file, lastClusterInChain, 0xFFF);
                break;
            }
        }
        else
        {
            /* Reset current and last clusters if the cluster is not empty */
            currentCluster = 0;
            lastCluster = 0;
        }
    }

    /* Return the cluster number of the first allocated cluster */
    return currentCluster;
}

void updateFAT(FILE *file, uint32_t cluster, uint16_t value)
{

    /* Calculate the offset in the FAT for the specified cluster */
    uint32_t fatOffset = fatoffset(0);
    fatOffset += 3 * (cluster / 2);

    /* Set the file position indicator to the FAT offset */
    fseek(file, fatOffset, SEEK_SET);

    /* Read the current FAT entry */
    uint8_t buffer[3];
    fread(buffer, 1, 3, file);

    /* Update the FAT entry based on the cluster's position in the entry */
    if (cluster % 2 == 0)
    {
        if (value == 0xFFF)
        {
            buffer[0] = 0xFF;
            buffer[1] = 0x0F;
        }
        else
        {
            buffer[0] = value & 0xFF;
            buffer[1] = ((value >> 8) & 0x0F) | (buffer[1] & 0xF0);
        }
    }
    else
    {
        if (value == 0xFFF)
        {
            buffer[1] = 0xFF;
            buffer[2] = 0xFF;
        }
        else
        {
            buffer[1] = ((value << 4) & 0xF0) | (buffer[1] & 0x0F);
            buffer[2] = (value >> 4) & 0xFF;
        }
    }

    /* Set the file position indicator back to the FAT offset */
    fseek(file, fatOffset, SEEK_SET);

    /* Write the updated FAT entry */
    fwrite(buffer, 1, 3, file);
}
