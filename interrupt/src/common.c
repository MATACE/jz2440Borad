#include "common.h"

void delay(volatile unsigned int time)
{
    while (time--);
}