#include "common.h"

void delay(volatile unsigned int time)
{
    while (time--)
        ;
}

char * strcpy(char *dest, const char *src)
{
    char *tmp = dest;

    while ((*dest++ = *src++) != '\0')
    /* nothing */;
    return tmp;
}

int strlen(const char *s)
{
    const char *sc;

    for (sc = s; *sc != '\0'; ++sc)
        /* nothing */;
    return sc - s;
}

void* memset(void* dst, int val, int count)
{
    void* ret = dst;
    while(count--)
    {
        *(char*)dst = (char)val;
        dst = (char*)dst + 1;
    }
    return ret;
}

