#include "s3c2440_soc.h"

void delay(volatile unsigned int time)
{
    while(time--);
}

int main()
{
    /* 设置GPIO配置寄存器 GPF4/5/6为输出 */
    GPFCON &= ~(3 << 8 | 3 << 10 | 3 << 12);
    GPFCON |= (1 << 8 | 1 << 10 | 1 << 12);

    /* 设置GPIO管脚的电平循环点亮 */
    unsigned int val = 0;
    // P_GPFDAT &= ~(1 << 4 | 1 << 5 | 1 << 6);
    // P_GPFDAT |= (5 << 4);
    
    while(1) {
        GPFDAT &= ~(1 << 4 | 1 << 5 | 1 << 6);
        GPFDAT |= (val << 4);
        val++;
        if (val == 8) {
            val = 0;
        }
        delay(100000);
    }

    return 0;
    
}