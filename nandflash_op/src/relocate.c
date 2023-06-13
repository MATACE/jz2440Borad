#include "relocate.h"

int is_nor_flash_boot(void)
{
    /* Norflash无法直接写入数据 */
    volatile unsigned int *p = (volatile unsigned int *)0;
    unsigned int val = *p;
    *p = 0x12345678;
    if (*p != 0x12345678) {
        /* 写入值和之前的值相同, Norflash启动 */
        return 1;
    } else {
        /* 写入值和之前的值不同, Nandflash启动, 恢复原来的值 */
        *p = val;
        return 0;
    }
}

void copy2sdram(void)
{

    extern int __code_start, __bss_start;

    volatile unsigned int *dest = (volatile unsigned int *)&__code_start;
    volatile unsigned int *end = (volatile unsigned int *)&__bss_start;
    volatile unsigned int *src = (volatile unsigned int *)0;

    unsigned int len = ((int)&__bss_start) - ((int)&__code_start);

    if (is_nor_flash_boot()) {
        /* Norflash启动 */
        while (dest < end) {
            *dest++ = *src++;
        }
    } else {
        /* Nandflash启动 */
        nand_init();
        read_nand((unsigned int)src, (unsigned char *)dest, len);
    }

}

void clean_bss(void)
{
    extern int _end, __bss_start;

    volatile unsigned int *start = (volatile unsigned int *)&__bss_start;
    volatile unsigned int *end = (volatile unsigned int *)&_end;

    while (start <= end)
    {
        *start++ = 0;
    }
}
