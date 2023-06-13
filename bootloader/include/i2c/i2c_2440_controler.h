#ifndef I2C_2440_CONTROLER_H
#define I2C_2440_CONTROLER_H

#include "s3c2440_soc.h"
#include "i2c_controler.h"
#include "interrupt.h"

extern struct i2c_msg;

void i2c_handle_func(const unsigned int irq_no);
int do_master_tx(struct i2c_msg * msg);
int do_master_rx(struct i2c_msg * msg);
int s3c2440_i2c_init(void);
int s3c2440_master_xfer(struct i2c_msg *msgs, int num);
int s3c2440_i2c_controller_add(void);

#endif // I2C_2440_CONTROLER_H

