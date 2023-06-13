#include "sdram.h"

void sdram_init(void)
{

    BWSCON &= ~((1 << 27) | (1 << 26) | (3 << 24));
    BWSCON |= (1 << 27) | (1 << 26) | (2 << 24);

    /* BANKCON6 bit16-15:11 bit3-2:00 bit1-0:01  */
    BANKCON6 &= ~((3 << 15) | (3 << 2) | (3 << 0));
    BANKCON6 |= (3 << 15) | (0 << 2) | (1 << 0);

    /* REFRESH bit23:0 bit22:0 bit21-20:00 bit19-18:01 bit10-0:0x4f5 */
    REFRESH &= ~((1 << 23) | (1 << 22) | (3 << 20) | (3 << 18) | (2047 << 0));
    REFRESH |= (0 << 23) | (0 << 22) | (0 << 20) | (1 << 18) | 0x4f5;

    /* BANKSIZE bit7:1 bit5:1 bit4:1 bit2-0:001*/
    BANKSIZE &= ~((1 << 7) | (1 << 5) | (1 << 4) | (7 << 0));
    BANKSIZE |= (1 << 7) | (1 << 5) | (1 << 4) | (1 << 0);

    /* MRSRB6 bit9:0 bit8-7:00 bit6-4:010或011 bit3:0 bit2-0:000 */
    MRSRB6 &= ~((1 << 9) | (3 << 7) | (7 << 4) | (1 << 3) | (7 << 0));
    MRSRB6 |= (0 << 9) | (0 << 7) | (2 << 4) | (0 << 3) | (0 << 0);
}

int sdram_test(void)
{
    volatile unsigned char *p = (volatile unsigned char *)0x30000000;

    // int i = 0;

    /* 连续数据读写 */

    // // write sdram
    // for (i = 0; i < 1000; i++) {
    //     p[i] = 0x55;
    // }
    
    // // read sdram
    // for (i = 0; i < 1000; i++) {
    //     if (p[i] != 0x55) {
    //         return -1;
    //     }
    // }
   
    /* 单个数据字节的读取 */
    p[32] = 0x32;
    if (p[32] != 0x32) {
        return -1;
    }

    return 0;
}
