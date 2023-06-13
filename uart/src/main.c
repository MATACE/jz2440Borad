#include "s3c2440_soc.h"
#include "led.h"
#include "uart.h"

int main()
{
    /* LED和按键寄存器初始化 */
    key_led_init();

    /* 初始化串口 */
    uart0_init();
    unsigned char ch = ' ';
    
    /* 串口打印数据 */
    puts("Hello World\n");

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