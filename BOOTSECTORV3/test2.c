#include "FAT_Parse/BootSector/bootSector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Display/Display.h" 

#include <windows.h> 

FILE* file;

int main()
{
    // if (argc != 2) {
    //     printf("Usage: %s \n", argv[0]);
    //     return 1;
    // }

    // // loadingBar();
    // system("cls");

    file = fopen("floppy.img", "r+b");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }


    // int numEntries = 16;
    struct ListNode *head = NULL;

    // readEntriesFromFile(file, 0x2600);

    // printList(head);
    printf("head:%x", head);
    fileSystemManager(file);

    struct ListNode *current = head;
    struct ListNode *next;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }

    fclose(file);

    return 0;
}
