#include "s3c2440_soc.h"
#include "led.h"
#include "uart.h"
#include "norflash.h"
#include "sdram.h"

int main()
{
    int ret = 0;

    /* LED和按键寄存器初始化 */
    key_led_init();

    /* 初始化串口 */
    uart0_init();
    unsigned char ch = ' ';
    
    /* 串口打印数据 */
    puts("Hello World\n");
    
    /* norflash的时钟设置 */
    ret = norflash_clock_set(5);
    if (ret < 0) {
        puts("Set norflash val not valid.\n");
    }

    /* sdram存储初始化 */
    sdram_init();

    /*测试sdram的读和存*/
    ret = sdram_test();
    if (ret < 0) {
       puts("Sdram test failed.\n");
    }

    /* 按键控制灯的亮灭 */
    while (1) {
        /* 按键监听 */
        // key_action();
        /* 串口接受 */
        ch = getchar();
        if (ch == '\r') {
            putchar('\n');
        }
        
        if (ch == '\n') {
            putchar('\r');
        }

        putchar(ch);
    }

    return 0;
    
}