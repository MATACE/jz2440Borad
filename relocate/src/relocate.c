#include "relocate.h"

/* NorFlash可以直接读取, NorFlash不可以直接去写 */

/** 代码段(.text 存放代码) 
 *  数据段(.data 全局变量) 
 *  只读数据段(.rodata 只读数据)
 *  
 *  程序通过手动把Bss段的数据清零，不保存在bin文件中
 *  Bss段(初值为0或无初值的全局变量) 不保存在bin文件中
 *  注释段(GCC的注释) 不保存在bin文件中 */

/* 全局变量保存在data数据段导致程序非常的大*/
unsigned char c_ch = 'A';
unsigned char c_ch1 = 'B';
const unsigned char c_con_ch = 'B';
unsigned char ch_a;
unsigned char ch_b;

void cpy2sdram_l(void)
{
    /** 从链接脚本lds中获取 code_start, bss_start的地址 
     *  然后从0地址把数据复制到code_start
     */

    extern unsigned int code_start, bss_start;
    volatile unsigned int *dest = (volatile unsigned int *)&code_start;
    volatile unsigned int *end = (volatile unsigned int *)&bss_start;
    volatile unsigned int *src = (volatile unsigned int *)0;
    
    while (dest < end) {
        *dest = *src;
        dest++;
        src++;
    }

}

void bss_clean_l(void)
{
    extern unsigned int bss_end, bss_start;
    volatile unsigned int *end = (volatile unsigned int *)&bss_end;
    volatile unsigned int *start = (volatile unsigned int *)&bss_start;
    while (start <= end) {
        *start = 0x0;
        start++;
    }
}

void cpy2sdram(volatile unsigned int *src, volatile unsigned int *dest, volatile unsigned int len)
{
    unsigned int i = 0;
    
    while (i < len) {
        *dest = *src;
        dest++;
        src++;
        i += 4;
    }
    
}

void bss_clean(volatile unsigned int *start, volatile unsigned int *end)
{
    while (start <= end) {
        *start = 0x0;
        start++;
    }
}

