#include "norflash.h"

int norflash_clock_set(int val)
{
    /* 查看主芯片的时序 */
    /* 查看norflash芯片的datasheet */
    /* 设置Tacc需要的时许 >= 70 ns */
    /* HCLK 100M 1/100 * 1000ms *1000us *1000ns = 10ns */
    /* Tacc >= 70 Tacc = 8clock, 10clock, 14clock */

    if (val != 5 && val != 6 && val != 7) {
        BANKCON0 &= ~(7 << 8);
        BANKCON0 |= (7 << 8);
        return -1;
    }

    if (val == 5) {
        BANKCON0 &= ~(7 << 8);
        BANKCON0 |= (5 << 8);
    } else if (val == 6) {
        BANKCON0 &= ~(7 << 8);
        BANKCON0 |= (6 << 8);
    } else if (val == 7) {
        BANKCON0 &= ~(7 << 8);
        BANKCON0 |= (7 << 8);
    }

    return 0;
}