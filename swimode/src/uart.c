#include "s3c2440_soc.h"

#include "uart.h"

void uart0_init(void)
{
    /* 配置GPH2/GPH3管脚为uart0功能 */
    GPHCON &= ~((3 << 4) | (3 << 6));
    GPHCON |= (2 << 4) | (2 << 6);

    /* uart0管脚上拉 */
    GPHDAT &= ~((1 << 2) | (1 << 3));

    /* 设置波特率 */
    /** UBRDIVn = (int)(UART clock / (buad rate * 16)) - 1 
     *  UART clock = 50M
     *  UBRDIVn = (int)((50000000) / (115200 * 16)) - 1 = 26
     *  
     */

    /* 设置串口的时钟和波特率 */
    UCON0 = 0x5; /* PCLK,中断/查询模式 */
    UBRDIV0 = 26;

    /* 设置串口的数据模式 8n1: 8个数据位 无校验位 1个停止位 */
    ULCON0 = 0x3; 

    puts("uart init OK\r\n");
}

int getchar(void)
{
    /* 串口状态数据寄存器 UTRSTAT0*/
    while (!((UTRSTAT0 >> 0) & 1));
    
    /* 串口数据寄存器 */
    return URXH0;
}

int putchar(int ch)
{
    /* 串口状态数据寄存器 UTRSTAT0*/
    while (!((UTRSTAT0 >> 2) & 1));
    
    /* 串口数据寄存器 */
    UTXH0 = (unsigned char)ch;

    return 0;
}

int puts(const char *s)
{
    while (*s) {
        if (*s == '\n') {
            putchar('\r');
            putchar('\n');
        } else {
            putchar(*s);
        }  
        s++;
    }

    return 0;
}

void print_hex(unsigned int val)
{
    int i = 0;
    unsigned char arr[8] = {};

    for (i = 0; i < 8; i++) {
        arr[i] = val & 0xf;
        val >>= 4;
    }

    puts("0x");
    for (i = 7; i >= 0; i--) {
        if (arr[i] >= 0 && arr[i] <= 9)
            putchar(arr[i] + '0');
        else if (arr[i] >= 0xA && arr[i] <= 0xF)
            putchar(arr[i] - 0xA + 'A');
    }
}