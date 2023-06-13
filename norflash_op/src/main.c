#include "uart.h"
#include "sdram.h"
#include "relocate.h"
#include "common.h"
#include "led.h"
#include "interrupt.h"
#include "timercount.h"
#include "my_printf.h"
#include "norflash.h"

int main(void)
{
    int ret = 0;

    /* LED初始化 */
    led_init();

    /* 中断控制器初始化 */
    // interrupt_control_init();

    /* 按键中断源初始化 */
    interrupt_key_init();

    /* timer0中断初始化  */
    /* 操作norflash时中断读取norflash程序导致失败 */
    // timer_init();

    unsigned char ch = ' ';

    /* printf测试 */
    // my_printf_test();

    /* norflash的时钟设置 */
    // ret = norflash_clock_set(5);
    // if (ret < 0) {
    //     puts("Set norflash val not valid.\n");
    // }

    /* sdram存储初始化 */
    // sdram_init();

    /*测试sdram的读和存*/
    // ret = sdram_test();
    // if (ret < 0) {
    //    puts("Sdram test failed.\n");
    // }

    norflash_op_menu();

    while (1)
    {
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
