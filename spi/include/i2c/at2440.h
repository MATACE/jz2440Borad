#ifndef AT2440_H
#define AT2440_H

#include "i2c_controler.h"

int at2440_write(unsigned int addr, unsigned char *data, int len);
int at2440_read(unsigned int addr, unsigned char *data, int len);

#endif // AT2440_H

