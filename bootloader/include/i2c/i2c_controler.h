#ifndef I2C_CONTROLER_H
#define I2C_CONTROLER_H

#include "common.h"
#include "my_printf.h"
#include "string_utils.h"
#include "i2c_2440_controler.h"

typedef struct i2c_msg {
    unsigned int cnt;
    int err;
    unsigned int addr;  /* 7bits */
    int flags;          /* 0-write, 1-read */
    int len;
    unsigned char *buf;
}I2C_MSG, *P_I2C_MSG;

typedef struct i2c_controller {
    int (*init)(void);
    int (*master_xfer)(P_I2C_MSG msgs, int num);
    char *name;
}I2C_CONTROL, *P_I2C_CONTROL;

int regisger_i2c_controller(P_I2C_CONTROL controller);
int select_i2c_controller(char *name);
int i2c_transfer(P_I2C_MSG msgs, int num);
void i2c_init(void);

#endif // I2C_CONTROLER_H

