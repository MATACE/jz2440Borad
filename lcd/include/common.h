#ifndef COMMON_H
#define COMMON_H

#define NULL  ((void*)0)

void delay(volatile unsigned int time);
int strlen(const char *s);
void* memset(void* dst, int val, int count);

#endif //COMMON_H
