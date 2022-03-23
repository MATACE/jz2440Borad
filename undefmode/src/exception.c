#include "exception.h"

void info_exception(unsigned int cpsr, char *str)
{
    puts("Exception! cpsr = ");
    print_hex(cpsr);
    puts(" ");
    puts(str);
    puts("\n\r");
}