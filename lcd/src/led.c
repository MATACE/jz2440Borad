
#include "led.h"

void led_init(void)
{
    /* 设置LED */
    /* 设置GPIO配置寄存器 GPF4/5/6为输出 */
    GPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));
    GPFCON |= ((1 << 8) | (1 << 10) | (1 << 12));
}

void key_led_init(void)
{
    /* 设置LED */
    /* 设置GPIO配置寄存器 GPF4/5/6为输出 */
    GPFCON &= ~((3 << 8) | (3 << 10) | (3 << 12));
    GPFCON |= ((1 << 8) | (1 << 10) | (1 << 12));

    /* 设置KEY */
    GPFCON &= ~((3 << 0) | (3 << 4));

    GPGCON &= ~(3 << 6);

    /* 熄灭所有灯 */
    GPFDAT &= ~(3 << 4 | 3 << 5 | 3 << 6);
    GPFDAT |= (1 << 4 | 1 << 5 | 1 << 6);
}

void key_action(void)
{
    unsigned int val = 0;

    /* 按键默认上拉输入高电平 */
        if (!((GPFDAT >> 0) & 1)) { //获取按键EINT0输入管脚的值
            /* 获取小灯的状态 */
            /* 消抖处理 时间大于10000小于20000间*/
            delay(10000);
            if (!((GPFDAT >> 0) & 1)) {
                val = (GPFDAT >> 4) & 1;
                if (val == 1) {
                    GPFDAT &= ~(1 << 4);
                } else if (val == 0) {
                    GPFDAT |= (1 << 4);
                }
            } 
        }  
}



