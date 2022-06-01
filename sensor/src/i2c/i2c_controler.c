#include "i2c_controler.h"

#define I2C_CONTROLLER_NUM 10

static P_I2C_CONTROL i2c_controller_arrays[I2C_CONTROLLER_NUM] = {};
static P_I2C_CONTROL p_i2c_controller = NULL;

int regisger_i2c_controller(P_I2C_CONTROL controller)
{
    /* 注册i2c控制器 */
    for (unsigned int i = 0; i < I2C_CONTROLLER_NUM; i++) {
        /* 在第一个非空的位置存放指针 */
        if (!i2c_controller_arrays[i]) {
            i2c_controller_arrays[i] = controller;
            return 0;
        }
    }

    /* 返回注册失败 */
    return -1;
}

/* 根据名字选择某款i2c控制器 */
int select_i2c_controller(char *name)
{
    /* 选择指定的i2c控制器 */
    for (unsigned int i = 0; i < I2C_CONTROLLER_NUM; i++) {
        if (i2c_controller_arrays[i] && !strcmp(i2c_controller_arrays[i]->name, name)) {
            /* 设置函数指针 */
            if (!p_i2c_controller) {
                p_i2c_controller = i2c_controller_arrays[i];
                return 0;
            }
        }
    }

    return -1;

}

/* i2c_transfer接口函数 */

int i2c_transfer(P_I2C_MSG msgs, int num)
{
    /* 调用i2c控制器接口 */
    if (p_i2c_controller) {
        return p_i2c_controller->master_xfer(msgs, num);
    }

    return -1;
}

void i2c_init(void)
{

    int ret = 0;

    /* 注册特定的i2c控制器 */
    s3c2440_i2c_controller_add();

    /* 选择特定的i2c控制 */
    ret = select_i2c_controller("s3c2440");

    // printf("select_i2c_controller=%d\r\n", ret);

    /* 调用i2c控制器的初始化函数 */
    p_i2c_controller->init();

}


