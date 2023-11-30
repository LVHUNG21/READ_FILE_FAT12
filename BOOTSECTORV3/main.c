/******************************************************************************
 * Include
 *****************************************************************************/
#include "FAT_Parse/BootSector/bootSector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Display/Display.h"
#include <windows.h>

/******************************************************************************
 * Global variabe
 *****************************************************************************/
FILE *file;

/******************************************************************************
 * Main
 *****************************************************************************/
int main()
{
    file = fopen("floppy.img", "r+b");

    if (file == NULL)
    {
        printf("Error opening file");
        return 1;
    }
    struct ListNode *head = NULL;

    fileSystemManager(file);

    /* free linker list after end of program */
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
/******************************************************************************
 * EOF
 *****************************************************************************/
