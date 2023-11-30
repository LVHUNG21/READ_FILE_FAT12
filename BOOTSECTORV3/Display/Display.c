#include "Display.h"

// #include "FAT_Parse/BootSector/bootSector.h"
#include <time.h>

#define TOTAL_CLUSTER 1200
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

ListNode *addNode1(ListNode *head, uint32_t address)
{
    /* Allocate memory for a new ListNode */
    ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));

    /* Check if memory allocation was successful */
    if (newNode == NULL)
    {
        /* If memory allocation failed, set newNode to NULL
         * to indicate the failure. */
        newNode = NULL;
    }

    /* Assign the address value to the newNode */
    newNode->address = address;

    /* Set the next pointer of the newNode to the current head */
    newNode->next = head;

    /* Return the newNode as the new head of the linked list */
    return newNode;
};
struct ListNode *deleteNode(struct ListNode *head, uint32_t addressToDelete)
{
    /* If the linked list is empty, return NULL */
    if (head == NULL)
    {
        return NULL;
    }

    /* Store the next node after the head as the new head */
    struct ListNode *newHead = head->next;

    /* Free the memory occupied by the current head */
    free(head);

    /* Return the new head of the linked list */
    return newHead;
}
int isValidFileEntry(DirectoryEntry *entry)
{
    /* Check if the first character of the entry's name is not a null character (0x00),
     * not a deleted file marker (0xE5), and the lower 4 bits of the attributes field are 0. */
    return (entry->name[0] != 0x00 && entry->name[0] != 0xE5 && (entry->attributes & 0x0F) == 0x00);
}

void handleOptionZr(struct ListNode **head, char *currentPath, uint8_t *exit, struct ListNode *current)
{
    /* Check if the address of the current node is 9728 */
    if (current->address == 9728)
    {
        /* Set the exit flag to 1 */
        *exit = 1;
    }
    else
    {
        /* Delete the current node from the linked list */
        *head = deleteNode(*head, current->address);

        /* Find the last occurrence of '/' character in currentPath */
        char *last = strrchr(currentPath, '/');
        if (last != NULL)
        {
            /* Set the last '/' character to null character ('\0') */
            *last = '\0';
        }

        /* Remove any spaces from currentPath */
        removeSpaces(currentPath);

        /* Display a loading bar with the updated currentPath */
        loadingBar(currentPath, 'b');
    }
}

void fileSystemManager(FILE *file)
{
    /* local variable */
    FILE_HANDLING_STATUS status = FILE_HAND_CREATE_FILE_OK;
    int index;
    uint32_t rootAddress = 0;
    calculateRootDirectoryAddress(&rootAddress);
    uint32_t option;
    /* Create a linked list with the initial node pointing to the root directory address */
    struct ListNode *head = addNode1(NULL, 9728);
    uint8_t exit = 0;
    char currentPath[100] = "Root";
    char formattedName[13];
    char filename[12];
    uint8_t multicluster = 'n';
    uint32_t countifmulti = 0;
    displayAppStarting();

    do
    {
        updateCurrentPath(currentPath, formattedName);

        int entryCount = 0;
        int numEntries = 0;
        struct ListNode *current = head;

        /* Read the directory entries from the file at the current node's address */
        DirectoryEntry *currentList = readEntriesFromFile(file, current->address, &numEntries, multicluster);

        if (currentList == NULL)
        {
            /* if currentList null notify */
            option = printList(currentList, &numEntries, current->address, 'y', currentPath);
        }
        else
        {
            option = printList(currentList, &numEntries, current->address, 'n', currentPath);
        }

        if (option >= 1 && option <= numEntries)
        {

            uint32_t clusteroffse = clusteroffset(currentList[option - 1].start_cluster);

            if (currentList[option - 1].attributes == 0x10)
            {
                if (fatentry(file, currentList[option - 1].start_cluster) == 0xFFF)
                {
                    /* Add a new node to the linked list and update the current path */
                    head = addNode1(head, clusteroffse);
                    displayOpeningFolder(currentList[option - 1].name, currentPath);
                    multicluster = 'n';
                }
                else
                {
                    /* Add a new node to the linked list and set multicluster flag to 'y' */
                    head = addNode1(head, clusteroffse);
                    multicluster = 'y';
                }
            }
            else
            {
                /* Read the content of the file and display it */
                readContentAndDisplay(file, currentList[option - 1].start_cluster, currentList[option - 1].file_size, currentPath);
            }
        }
        else if (option == 0)
        {
            /* Handle the "Zr" option */
            handleOptionZr(&head, currentPath, &exit, current);
        }

        if (option == numEntries + 1)
        {
            /* Create a new file */
            char filename[12];
            getUserInputFilename(filename);

            uint32_t fileSize;
            getUserInputFileSize(&fileSize);

            /* Create the file with the given filename, size, and current directory address */
            status = createFile(file, filename, fileSize, current->address);
            displayStatusMessage(status);
        }
        if (option == numEntries + 2)
        {
            /* Create a new folder */
            char foldername[12];
            getUserInputFoldername(foldername);

            /* Create the folder with the given foldername and current directory address */
            status = createFolder(file, foldername, current->address);
            displayStatusMessage(status);
        }
        else if (option == numEntries + 3)
        {
            /* Delete a file or folder */
            int index = getUserInputFolderIndex();
            DirectoryEntry entry = currentList[index - 1];
            if (entry.attributes == 0x10)
            {
                /* Display the folder deletion message */
                displayFolderDeletionMessage(entry.name);
            }
            else
            {
                /* Delete the file with the given filename, current directory address, and entry information */
                status = deleteFile(file, filename, current->address, entry);
                displayStatusMessage(status);
            }
        }
        else if (option != 0 && option > numEntries + 3)
        {
            /* Display an invalid choice message */
            displayInvalidChoice();
        }
    } while (option != 0 || exit == 0);
}

void readContentAndDisplay(FILE *file, uint32_t startCluster, uint32_t fileSize, char path[100])
{
    uint8_t check = 'n';
    do
    {
        system("cls");

        uint32_t offset = clusteroffset(startCluster);

        fseek(file, offset, SEEK_SET);
        if (1)
        {
            readContentFromClusters(file, startCluster, fileSize);
        }
        check = backOption();

    } while (check != 'n');
    loadingBar(path, 'b');
}
void readContentFromClusters(FILE *file, uint32_t startCluster, uint32_t fileSize)
{

    uint16_t SecPerClus = getSecPerClus();
    uint16_t BytePerSec = getBytsPerSec();
    uint32_t clusterSize = SecPerClus * BytePerSec;
    uint8_t *buffer = (uint8_t *)malloc(clusterSize);
    uint32_t bytesRead = 0;
    uint32_t currentCluster = startCluster;
    uint32_t offset = 0;

    while (currentCluster < 0xFF8 && fileSize > 0)
    {
        offset = clusteroffset(currentCluster);

        fseek(file, offset, SEEK_SET);
        bytesRead = fread(buffer, 1, clusterSize, file);

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
