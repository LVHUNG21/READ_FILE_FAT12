#include <stdio.h>
#include <stdlib.h>
struct Directory_Entry {
  char     name[8];       /* Space padded filename ([0] = E5h if deleted, 05h if pending delete) */
  char     ext[3];        /* File extension */
  uint8_t  attributes;    /* File attributes bitfield */
  uint8_t  reserved[10];  /* Reserved, system dependent uses */
  uint16_t created_time;  /* [15:11] = hours(0..23), [10:5] = minutes, [4:0] = seconds/2 */
  uint16_t created_date;  /* [15:9] = year - 1980, [8:5] = month, [4:0] = day */
  uint16_t start_cluster; /* Cluster where this directory entry starts */
  uint32_t file_size;     /* Size of the file in bytes */
};
printf("");

void fileSystemManager(FILE *file, struct BootSector *bs, struct ListNode *head)
{
    int option;

    do
    {
        printf("\nFile System Manager Menu:\n");

        struct ListNode *current = head;
        int entryCount = 0;

        while (current != NULL)
        {
            ++entryCount;
            printf("%d. %s\n", entryCount, current->data.name);
            current = current->next;
        }

        printf("%d. Create File\n", entryCount + 1);
        printf("%d. Create Folder\n", entryCount + 2);
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option >= 1 && option <= entryCount)
        {
            current = head;
            for (int i = 1; i < option; ++i)
            {
                current = current->next;
            }

            if (current->data.attributes & 0x10)
            {
                printf("\nOpening folder: %s\n", current->data.name);
                displayFolderContents(file, bs, current->data.start_cluster, current->data.file_size);
            }
            else
            {
                printf("\nOpening file: %s\n", current->data.name);
                readContentAndDisplay(file, bs, current->data.start_cluster, current->data.file_size, 1);
            }
        }
        else if (option == entryCount + 1)
        {
            char filename[12];
            uint32_t fileSize;
            printf("Enter the filename: ");
            scanf("%s", filename);
            printf("Enter the file size: ");
            scanf("%u", &fileSize);
            createFile(file, bs, filename, fileSize);
        }
        else if (option == entryCount + 2)
        {
            char foldername[12];
            printf("Enter the folder name: ");
            scanf("%s", foldername);
            createFolder(file, bs, foldername);
        }
        else if (option != 0)
        {
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (option != 0);
}

void displayFolderContents(FILE *file, struct BootSector *bs, uint32_t startCluster, uint32_t fileSize)
{

    size_t clusterSize = 512;
    unsigned char buffer[clusterSize];

    uint32_t offset = clusteroffset(bs, startCluster);

    fseek(file, offset, SEEK_SET);

    struct ListNode *folderEntries = NULL;
    readEntriesFromFile(file, offset, &folderEntries, fileSize / 32);

    int option;

    do
    {
        printf("\nFolder Contents:\n");

        struct ListNode *current = folderEntries;
        int entryCount = 0;

        while (current != NULL)
        {
            ++entryCount;
            printf("%d. %s\n", entryCount, current->data.name);
            current = current->next;
        }

        printf("0. Go back to the parent folder\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option >= 1 && option <= entryCount)
        {

            current = folderEntries;
            for (int i = 1; i < option; ++i)
            {
                current = current->next;
            }

            if (current->data.attributes & 0x10)
            {
                printf("\nOpening folder: %s\n", current->data.name);

                displayFolderContents(file, bs, current->data.start_cluster, current->data.file_size);
            }
            else
            {
                printf("\nOpening file: %s\n", current->data.name);
                readContentAndDisplay(file, bs, current->data.start_cluster, current->data.file_size, 1);
            }
        }
        else if (option != 0)
        {
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (option != 0);

    struct ListNode *current = folderEntries;
    struct ListNode *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

void displayFolderContents(FILE *file, struct BootSector *bs, uint32_t startCluster, uint32_t fileSize)
{

    size_t clusterSize = 512;
    unsigned char buffer[clusterSize];

    uint32_t offset = clusteroffset(bs, startCluster);

    fseek(file, offset, SEEK_SET);

    struct ListNode *folderEntries = NULL;
    readEntriesFromFile(file, offset, &folderEntries, fileSize / 32);

    int option;

    do
    {
        printf("\nFolder Contents:\n");

        struct ListNode *current = folderEntries;
        int entryCount = 0;

        while (current != NULL)
        {
            ++entryCount;
            printf("%d. %s\n", entryCount, current->data.name);
            current = current->next;
        }

        printf("0. Go back to the parent folder\n");
        printf("Enter your choice: ");
        scanf("%d", &option);

        if (option >= 1 && option <= entryCount)
        {

            current = folderEntries;
            for (int i = 1; i < option; ++i)
            {
                current = current->next;
            }

            if (current->data.attributes & 0x10)
            {
                printf("\nOpening folder: %s\n", current->data.name);

                displayFolderContents(file, bs, current->data.start_cluster, current->data.file_size);
            }
            else
            {
                printf("\nOpening file: %s\n", current->data.name);
                readContentAndDisplay(file, bs, current->data.start_cluster, current->data.file_size, 1);
            }
        }
        else if (option != 0)
        {
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (option != 0);

struct ListNode *current = folderEntries;
struct ListNode *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}
