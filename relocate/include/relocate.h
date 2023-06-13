#ifndef RELOCARE_H
#define RELOCARE_H

#include "s3c2440_soc.h"

void cpy2sdram(volatile unsigned int *src, volatile unsigned int *dest, volatile unsigned int len);
void bss_clean(volatile unsigned int *start, volatile unsigned int *end);

void cpy2sdram_l(void);
void bss_clean_l(void);

#endif // RELOCARE_H