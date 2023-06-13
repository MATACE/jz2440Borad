#ifndef RELOCARE_H
#define RELOCARE_H

#include "s3c2440_soc.h"
#include "nandflash.h"

int is_nor_flash_boot(void);
void copy2sdram(void);
void clean_bss(void);

#endif // RELOCARE_H
