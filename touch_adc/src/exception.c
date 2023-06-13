
#include "uart.h"

void info_exception(unsigned int cpsr, char *str)
{
	puts("Exception! cpsr = ");
	print_hex(cpsr);
	puts(" ");
	puts(str);
	puts("\n\r");
}

void info_swi_exception(unsigned int *ps_swi)
{
    puts("SWI val = ");
    print_hex(*ps_swi & ~0xff000000);
    puts("\n\r");
}
