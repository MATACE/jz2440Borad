#include "at2440.h"

#define AT24CXX_ADDR 0x50

int at2440_write(unsigned int addr, unsigned char *data, int len)
{
    /* 构造i2c_msg结构体 */
    I2C_MSG msg = {};
    int err = 0;
    unsigned char buf[2] = {};

    for (unsigned int i = 0; i < len; i++) {
        buf[0] = addr++;
        buf[1] = data[i];
        msg.addr = AT24CXX_ADDR;
        msg.flags = 0; /* write */
        msg.len = 2;
        msg.buf = buf;
        msg.err = 0;
        msg.cnt = -1;

        /* 调用i2c_transfer */
        i2c_transfer(&msg, 1);
    }

    if (err) {
        return err;
    }

    return 0;
}

int at2440_read(unsigned int addr, unsigned char *data, int len)
{
    I2C_MSG msg[2] = {};
    int err = 0;

    /* 构造i2c_msg结构体 */

    /* 发送设备地址 */
    msg[0].addr = AT24CXX_ADDR;
    msg[0].flags = 0; /* write */
    msg[0].len = 1;
    msg[0].buf = (unsigned char *)&addr;
    msg[0].err = 0;
    msg[0].cnt = -1;

    /* 获取设备内存地址的数据 */
    msg[1].addr = AT24CXX_ADDR;
    msg[1].flags = 1; /* read */
    msg[1].len = len;
    msg[1].buf = data;
    msg[1].err = 0;
    msg[1].cnt = -1;

    /* 调用i2c_transfer */
    i2c_transfer(&msg, 2);
    if (err) {
        return err;
    }

    return 0;
}


