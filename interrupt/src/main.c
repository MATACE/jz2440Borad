#include "uart.h"
#include "sdram.h"
#include "relocate.h"
#include "common.h"
#include "led.h"
#include "interrupt.h"

int main(void)
{
    int ret = 0;

    /* LED初始化 */
    led_init();

    interrupt_control_init();

    interrupt_key_init();

    unsigned char ch = ' ';

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
