#include "photoresistance.h"

unsigned int PHOTORESISTANCE_FLAG = 0;

void photo_gpio_init()
{
    /* 设置GPG7为中断检测光敏电阻和可变电阻电压值 */
    GPGCON &= ~(3 << 14);
    GPGCON |= 2 << 14;

    /* 设置边沿触发方式 */
    EXTINT1 &= ~(7 << 28);
    EXTINT1 |= (7 << 28);

    /* 使能EINTMASK中断屏蔽位 */
    EINTMASK &= ~(1 << 15);

    /* 注册ENI15的中断函数,在之前的按键中断中修改 */

}

void get_photoresistance_val(void)
{
    unsigned int m1 = 0, m0 = 0;
    unsigned int n1 = 0, n0 = 0;
    double vol1 = 0, vol0 = 0;
    unsigned int val1 = 0, val0 = 0;

    /* 获取光敏电阻AIN1 */
    val1 = get_ain1_adc_val();
    vol1 = (double)val1 / 1023 * 3.3;
    m1 = (int)vol1;
    n1 = (vol1 - m1) * 1000;
    // printf("AIN1: %d.%02dv\r\n", m1, n1);

    /* 获取可调电阻AIN0 */
    val0 = get_ain0_adc_val();
    vol0 = (double)val0 / 1023 * 3.3;
    m0 = (int)vol0;
    n0 = (vol0 - m0) * 1000;
    printf("AIN0: %d.%02dv AIN1: %d.%02dv\r", m0, n0, m1, n1);

    // if (PHOTORESISTANCE_FLAG) {
    //     printf("Light Down \r\n");
    // } else {
    //     printf("Light Up \r\n");
    // }

}



