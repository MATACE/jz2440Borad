#include "timercount.h"

TIMER_DESC timer_array[TIME_NUM] = {};

void timer_irq(const unsigned int irq_no)
{
    for (unsigned int i = 0; i < TIME_NUM; i++) {
        /* 判断函数是否为空 */
        if (timer_array[i].fp != NULL) {
            /* 执行函数 */
            timer_array[i].fp();
        }
    }

    return;
}

int register_timer(char *name, timer_func fp)
{
    for (unsigned int i = 0; i < TIME_NUM; i++) {
        /* 判断某一个为空时传入数据 */
        if (!timer_array[i].fp) {
            timer_array[i].name = name;
            timer_array[i].fp = fp;
            return 0;
        }
    }

    return -1;
}

int unregister_timer(char *name)
{
    for (unsigned int i = 0; i < TIME_NUM; i++) {
        /* 判断某一个为空时传入数据 */
        if (strcmp(timer_array[i].name, name) == 0) {
            timer_array[i].name = NULL;
            timer_array[i].fp = NULL;
            return 0;
        }
    }
    return -1;
}

void timer0_touch_screen_irq_handle(const unsigned int irq_no)
{
    return;
}

void inter_contr_timer0_init(void)
{
    /* 设置中断控制器的INTMSK,使能INT_TIMER0 */
    INTMSK &= ~(1 << 10);
}

void timer_init(void)
{
    /* 设置定时器的预分频频率 */
    /**
     * Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}
     * {prescaler value} = 0~255
     * {divider value} = 2, 4, 8, 16
     */
    TCFG0 &= ~(0xff << 0);
    TCFG0 |= 0x63;
    
    /* 设置定时器的分频选择器 */
    TCFG1 &= ~(0xf << 0);
    TCFG1 |= (2 << 0);
    
    /* 设置定时器的计数寄存器 */
    /**
     * Timer input clock Frequency 62500= 50000000 / (99 + 1) / 8
     * 1 / 62500 * 31250 = 0.5s
     */
    // TCNTB0 = 0x7a12;

     /**
     * Timer input clock Frequency 62500= 50000000 / (99 + 1) / 8
     * 1 / 62500 * 625 = 10ms
     */
    TCNTB0 = 0x217;
    
    /* 控制寄存器装载计数寄存器 */
    TCON |= (1 << 1);

    /* 设置定时器自动装载和启动定时器 */
    TCON &= ~((1 << 0) | (1 << 1) | (1 << 3));
    TCON |= (1 << 0) | (1 << 3);

    /* 使能中断控制器的定时器中断 */
    // inter_contr_timer0_init();

    /* 注册中断控制器 */
    register_irq_func(10, timer_irq);

}


