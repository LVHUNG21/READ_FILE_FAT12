#include "io.h"
void loadingBar(char str[20], uint8_t type);
void removeSpaces(char *str)
{
    int i, j = 0;
    int len = strlen(str);
    for (i = 0; i < len; i++)
    {
        if (!isspace((uint8_t)str[i]))
        {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
}
uint32_t printList(struct DirectoryEntry *entries, uint32_t *entryCont, uint32_t address, uint8_t check_null, char currenpath[100])
{
    system("cls");
    printf("\n\t\t\t\t\t\t\t\t\t\033[36m*********  FAT12 MANAGEMENT SYSTEM USING C **********\033[0m\n");
    uint32_t option = 0;
    printf("\n");

    printf("\n\t\t\t\t\t\t\033[32mPath:%s\033[0m\n", currenpath);
    printf("\033[1;36m\n\t\t\t\t\t\t|====================|====================|====================|====================|==============================|\033[0m\n");
    printf("\t\t\t\t\t\t|                    |                    |                    |                    |                              |");
    printf("\n\t\t\t\t\t\t| \033[33m%-18s\033[0m | \033[33m%-18s\033[0m | \033[33m%-18s\033[0m | \033[33m%-18s\033[0m | \033[33m%-18s\033[0m           |\n", "Index", "Name", "Type", "Size", "Created");
    printf("\t\t\t\t\t\t|                    |                    |                    |                    |                              |");
    printf("\033[1;36m\n\t\t\t\t\t\t|====================|====================|====================|====================|==============================|\033[0m\n");
    if (check_null == 'y')
    {
        printf("\t\t\t\t\t\t\t\t\t \033[1;31m ********* THIS FOLDER IS EMPTY (create or back)**********\033[0m\n\n");
    }
    else
    {
        for (int i = 0; i < *entryCont; i++)
        {
            union DateTime createdDateTime;
            createdDateTime.packed = entries[i].created_date;

            union Time createdTime;
            createdTime.packed = entries[i].created_time;

            char formattedName[13];
            strncpy(formattedName, entries[i].name, 8);
            formattedName[8] = '\0';

            if (entries[i].attributes == 0x10)
            {
                printf("\033[1;36m\t\t\t\t\t\t|                    |                    |                    |                    |                              |\033[0m");
                printf("\n\t\t\t\t\t\t| \033[1;36m%-5d              | %-19s| %-13s      | %-18s | %04d-%02d-%02d %02d:%02d:%02d          |\033[0m\n", i + 1, formattedName, (entries[i].attributes & 0x10) ? "Folder" : "File", "", createdDateTime.fields.year + 1980, createdDateTime.fields.month, createdDateTime.fields.day, createdTime.fields.hour, createdTime.fields.minute, createdTime.fields.second);
            }
            else
            {
                printf("\033[1;36m\t\t\t\t\t\t|                    |                    |                    |                    |                              |\033[0m");
                printf("\033[1;36m\n\t\t\t\t\t\t| %-5d              | %-19s| %-13s      | %-19u| %04d-%02d-%02d %02d:%02d:%02d          |\033[0m\n", i + 1, entries[i].name, (entries[i].attributes & 0x10) ? "Folder" : "File", entries[i].file_size, createdDateTime.fields.year + 1980, createdDateTime.fields.month, createdDateTime.fields.day, createdTime.fields.hour, createdTime.fields.minute, createdTime.fields.second);
            }
        }
    }
    printf("\033[1;36m\t\t\t\t\t\t|====================|====================|====================|====================|==============================|\033[0m\n");
    printf("\033[1;33m\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t[%d]:Create File  \n\033[0m", *(entryCont) + 1);
    printf("\033[1;33m\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t[%d]:Create Folder (meta-updating)\n\033[0m", *(entryCont) + 2);
    if (entries != NULL)
    {
        printf("\033[1;33m\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t[%d]:delete file\n\033[0m", *(entryCont) + 3);
    }
    if (address != 9728)
    {
        printf("\033[1;33m\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t[0].Back to parent folder !!\n\033[0m");
    }
    else
    {
        printf("\033[1;33m\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t[0]. Exit\n\033[0m");
    }
    printf("\n\t\t\t\t\t\tEnter your choice: ");
    fflush(stdin);

    uint32_t inputCount = scanf(" %d", &option);

    while (inputCount != 1)
    {
        printf("\n\t\t\t\t\t\tInvalid input. Please enter a valid integer: ");
        /* delete old data */
        while (getchar() != '\n')
            ;
        inputCount = scanf(" %d", &option);
    }
    return option;
}
uint8_t backOption()
{
    uint8_t check = 'n';
    uint8_t exit = 1;
    fflush(stdin);
    printf("\n\n\n\t\t\t\t\t\t\033[32mDo you want to display again: \033[0m");

    while (exit == 1)
    {
        fflush(stdin);
        scanf("%c", &check);

        if (check == 'y' || check == 'n')
        {
            exit = 0;
        }
        else
        {
            printf("Invalid input. Please enter 'y' or 'n': ");
        }
    }

    return check;
}
void displayOpeningFolder(char *folderName, char *currentPath)
{
    printf("\nOpening folder: %s\n", folderName);
    char formattedName[13];
    strncpy(formattedName, folderName, 8);
    formattedName[8] = '\0';
    strcat(currentPath, "/");
    strcat(currentPath, formattedName);
    removeSpaces(currentPath);
    loadingBar(currentPath, 'f');
}

void displayOpeningFile(char *fileName, char *currentPath)
{
    printf("\nOpening file: %s\n", fileName);
}

void displayInvalidChoice()
{
    printf("\n\t\t\t\t\t\tInvalid choice. Please enter a valid option");
    Sleep(2000);
}
void displayCurrentPath(char *currentPath)
{
    printf("Current Path: %s\n", currentPath);
}

void getUserInputFilename(char *filename)
{
    printf("\n\n\t\t\t\t\t\tEnter the filename: ");
    scanf("%s", filename);
}

void getUserInputFileSize(uint32_t *fileSize)
{
    printf("\t\t\t\t\t\tEnter the file size: ");
    scanf("%u", fileSize);
}

void displayFolderDeletionPrompt()
{
    printf("\t\t\t\t\t\tEnter index of the folder you want to delete: ");
}

void displayFolderDeletionMessage(char *folderName)
{
    system("cls");
    printf("\t\t\t\t\t\t\t\t'%s' is a folder. Please use 'file' to delete.\n", folderName);
}
void getUserInputFoldername(char *foldername)
{
    printf("\n\n\t\t\t\t\t\tEnter the folder name: ");
    scanf("%s", foldername);
}

int getUserInputFolderIndex()
{
    int index;
    printf("\t\t\t\t\t\tEnter index of the folder you want to delete: ");
    scanf("%d", &index);
    return index;
}
void displayFileContent(uint8_t *data, uint32_t dataSize)
{
    printf("Hexadecimal :%x\n", dataSize);


    printf("\nASCII \n");

    for (uint32_t i = 0; i < dataSize; ++i)
    {

        char character = (char)data[i];
        printf("%c", character);
    }

    printf("\n\n\n");
}
void displayAppStarting()
{
    loadingBar("jj", 'p');
    printf("\n\t\t\t\t\t\t\t\t\t ");
    char AppStarting[100] = "\033[36m*********  FAT12 MANAGEMENT SYSTEM USING C **********\033[0m\n";
    for (int i = 0; i < strlen(AppStarting); i++)
    {
        printf("%c", AppStarting[i]);
        Sleep(40);
    }
}
void updateCurrentPath(char *currentPath, char *formattedName)
{
    strcat(currentPath, formattedName);
    removeSpaces(currentPath);
    printf("\t\t\t\t\t\t\t%s", currentPath);
}
void getDataFromUser(uint8_t *buffer)
{
    printf("\n\n\t\t\t\t\t\tPlease enter content file:\n");
    scanf("%s", (char *)buffer);
}
void loadingBar(char str[20], uint8_t type)
{
    system("cls");
    system("color 0A");
    char a = 177, b = 219;

    if (type == 'f')
    {
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
        printf("\n\n\n\n\t\t\t\t\t\t\t\
           Open folder %s...\n\n",
               str);
        printf("\t\t\t\t\t\t\t\t");
    }
    else if (type == 'b')
    {
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
        printf("\n\n\n\n\t\t\t\t\t\t\t\
           Back to %s...\n\n",
               str);
        printf("\t\t\t\t\t\t\t\t");
    }
    else
    {
        printf("\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t");
        printf("\n\n\n\n\t\t\t\t\t\t\t\
           Start the program...\n\n");
        printf("\t\t\t\t\t\t\t\t");
    }

    for (int i = 0; i < 26; i++)
        printf("%c", a);

    printf("\r");
    printf("\t\t\t\t\t\t\t\t");

    for (int i = 0; i < 26; i++)
    {
        printf("%c", b);

        Sleep(50);
    }
    system("color 07");
    system("cls");
}
