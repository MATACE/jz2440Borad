#include "adc.h"

void adc_init(void)
{
    /**
     * ECFLG   bit15-1 转换flag成功
     * PRSCEN  bit14-1 预分频
     * PRSCVL  bit13:bit6-49 预分频值
     * SEL_MUX bit5:bit3- 选择输入ADC引脚
     * STDBM   bit2:1     待机模式
     * READ_ START bit1   可以读取值
     * ENABLE_START bit0   写入1转换使能,需要清除0
     */

    ADCCON = (1 << 14) | (49 << 6) | (0 << 3);

    ADCDLY = 0xff;

    return;
}

unsigned int get_ain0_adc_val(void)
{
    /* 获取ADC的值 */
    /* 开始ADC转换 */
    ADCCON |= 1;

    /* 等待获取ADC值, 完成 */
    while (!((ADCCON >> 15) & 1));

    /* 获取AIN0的寄存器的值 */
    return (ADCDAT0 & 0x3ff);
}



