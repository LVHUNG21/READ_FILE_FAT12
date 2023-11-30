#include "io2.h"
const char *statusMessages[] = {
    "\t\t\t\t\t\t\tFile created successfully!",
    "\t\t\t\t\t\t\tFailed to create file.",
    "\t\t\t\t\t\t\tFolder created successfully!",
    "\t\t\t\t\t\t\tFailed to create folder.",
    "\t\t\t\t\t\t\tFile deleted successfully!",
    "\t\t\t\t\t\t\tFailed to delete file."
};

void displayStatusMessage(FILE_HANDLING_STATUS status) {
    printf("%s\n", statusMessages[status]);
    Sleep(1000);
}