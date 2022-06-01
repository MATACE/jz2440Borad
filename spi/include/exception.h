#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "s3c2440_soc.h"

#include "uart.h"

void info_exception(unsigned int cpsr, char *str);
void print1(void);
void print2(void);
void info_swi_exception(unsigned int *ps_swi);

#endif // EXCEPTION_H
