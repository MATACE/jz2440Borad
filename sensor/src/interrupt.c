#include "interrupt.h"

irq_handle_func irq_func_array[64] = {};

extern unsigned int PHOTORESISTANCE_FLAG;

void register_irq_func(const unsigned int irq_no, irq_handle_func func)
{
    irq_func_array[irq_no] = func;

    /* 打开中断屏蔽标志位 */
     INTMSK &= ~(1 << irq_no);
}

void interrupt_control_init(void)
{
    /* 设置中断控制器的INTMSK,使能EINT0 EINT2 EINT11 EINT19 */
    INTMSK &= ~((1 << 0) | (1 << 2) | (1 << 5));
}

void interrupt_key_init(void)
{
    /* 中断EINT0 EINT2 EINT11 EINT19 */
    /* 配置GPF0/GPF2为中断引脚 */
    GPFCON &= ~((3 << 4) | (3 << 0));
    GPFCON |= (2 << 4) | (2 << 0);

    /* 配置GPG3/GPG11为中断引脚 */
    GPGCON &= ~((3 << 6) | (3 << 22));
    GPGCON |= (2 << 6) | (2 << 22);

    /* 设置ENIT0/EINT2中断的触发模式--下降沿触发 */
    EXTINT0 &= ~((7 << 0) | (7 << 8));
    EXTINT0 |= (3 << 0) | (3 << 8);

    /* 设置EINT11中断的触发模式--下降沿触发*/
    EXTINT1 &= ~(7 << 12);
    EXTINT1 |= (3 << 12);

    /* 设置EINT19中断的触发模式--下降沿触发*/
    EXTINT2 &= ~(7 << 12);
    EXTINT2 |= (3 << 12);      

    /* 使能EINTMASK中断屏蔽位 */
    EINTMASK &= ~((1 << 11) | (1 << 19));

    /* 注册中断处理函数 */
    register_irq_func(0, key_irq_handle);
    register_irq_func(2, key_irq_handle);
    register_irq_func(5, key_irq_handle);
}

void key_irq_handle(unsigned int irq_no)
{
    unsigned int eint_val = EINTPEND;
    unsigned int val = 0;

    if (irq_no == 0) {
        /* 改变小灯的状态 */
        val = (GPFDAT >> 4) & 1;
        if (val == 1) {
            GPFDAT &= ~(1 << 4);
        } else if (val == 0) {
            GPFDAT |= (1 << 4);
        }
    } else if (irq_no == 2) {
        /* 改变小灯的状态 */
        val = (GPFDAT >> 5) & 1;
        if (val == 1) {
            GPFDAT &= ~(1 << 5);
        } else if (val == 0) {
            GPFDAT |= (1 << 5);
        }
    } else if (irq_no == 5) {
        if (eint_val & (1 << 11)) {
            /* 按键 */
            val = (GPFDAT >> 6) & 1;
            if (val == 1) {
                GPFDAT &= ~(1 << 6);
            } else if (val == 0) {
                GPFDAT |= (1 << 6);
            }
        } else if (eint_val & (1 << 19)) {
            /* 按键 */
            GPFDAT &= ~((1 << 4) | (1 << 5) | (1 << 6));
        } else if (eint_val & (1 << 15)) {
            /* 光敏电阻的比较值 */
            /* 获取GPG7的数据 */
            val = (GPGDAT >> 7) & 1;
            if (val == 1) {
                PHOTORESISTANCE_FLAG = 1;
                printf("Light Down \r\n");
            } else if (val == 0) {
                PHOTORESISTANCE_FLAG = 0;
                printf("Light Up \r\n");
            }
        }
    }

    EINTPEND = eint_val;
}

void irq_handle(void)
{
    /* 分辨在中断控制中的中断源 */
    unsigned int irq_no = INTOFFSET;

    /* 调用对应的中断处理函数 */
    // if (irq_no == 0 || irq_no == 2 || irq_no == 5) {
    //     /* 按键中断处理函数 */
    //     // key_irq_handle(irq_no);
    //     /* 清除IO口的中断标志位  */
    // } else if (irq_no == 10) {
    //     /* timer0定时器中断处理函数 */
    //     timer0_irq_handle();
    // }
    
    /* 通过或指针函数数组来处理中断 */
    if (irq_func_array[irq_no] != NULL) {
        irq_func_array[irq_no](irq_no);
    }    

    /* 清除中断控制器的中断标志位 */
    SRCPND = (1 << irq_no);
    INTPND = (1 << irq_no);	
}

