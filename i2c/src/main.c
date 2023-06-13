#include "uart.h"
#include "sdram.h"
#include "relocate.h"
#include "common.h"
#include "led.h"
#include "interrupt.h"
#include "timercount.h"
#include "my_printf.h"
#include "norflash.h"
#include "nandflash.h"
#include "lcd_menu.h"
#include "i2c_menu.h"

extern unsigned int nand_init_flag;

int main(void)
{
    int ret = 0;

    /* 串口初始化 */
    // uart0_init();

    /* LED初始化 */
    led_init();

    /* 中断控制器初始化 */
    // interrupt_control_init();

    /* 按键中断源初始化 */
    interrupt_key_init();

    /* timer0中断初始化  */
    /* 操作norflash时中断读取norflash程序导致失败 */
    timer_init();

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

    /* Norflash操作 */
    norflash_op_menu();

    /* Nandflash操作 */
    if (nand_init_flag == 0) {
        nand_init();
    }
    nandflash_op_menu();

    /* Lcd操作 */
    lcd_menu();

    /* I2c操作 */
    i2c_menu();

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
