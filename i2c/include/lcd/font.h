#ifndef FONT_H
#define FONT_H

#include "font_char.h"

void font_init(void);
void print_font_char(unsigned int x, unsigned int y, unsigned char ch,unsigned int color);
void print_font_string(unsigned int x, unsigned int y, const char *buf, unsigned int color);

#endif // FONT_H


