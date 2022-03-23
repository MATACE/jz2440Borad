#ifndef SDRAM_H
#define SDRAM_H

#include "s3c2440_soc.h"

void sdram_init(void);
int sdram_test(void);

#endif // SDRAM_H