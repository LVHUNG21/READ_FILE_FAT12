#include "io2.h"
const char *statusMessages[] = {
    "\n\n\n\n\t\t\t\t\t\t\t\tFile created successfully!",
    "\n\n\n\n\t\t\t\t\t\t\t\tFailed to create file.",
    "\n\n\n\n\t\t\t\t\t\t\t\tFile delete successfully!",
    "\n\n\n\n\t\t\t\t\t\t\t\tFailed to create folder.",
    "\n\n\n\n\t\t\t\t\t\t\t\tFile deleted successfully!",
    "\n\n\n\n\t\t\t\t\t\t\t\tFailed to delete file."
};

void displayStatusMessage(FILE_HANDLING_STATUS status) {
    system("cls");
    printf("%s\n", statusMessages[status]);
    Sleep(1000);
}