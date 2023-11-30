#ifndef BOOTSECTORINFOPRINTER_H
#define BOOTSECTORINFOPRINTER_H

#include "BootSector.h"

/**
 * @brief Prints information from the Boot Sector structure.
 *
 * @param[in] bootSector A pointer to the Boot Sector structure.
 */
void printBootSectorInfo(struct BootSector *bootSector);

#endif /* BOOTSECTORINFOPRINTER_H */
