#ifndef RELOCARE_H
#define RELOCARE_H

#include "s3c2440_soc.h"

void copy2sdram(void);
void clean_bss(void);

#endif // RELOCARE_H