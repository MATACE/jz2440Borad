#include "spi_controler_reg.h"

void spi_control_init(void)
{
    /* 初始化SPI的GPIO管脚 */
    spi_control_gpio_init();
    
    /* 设置SPI控制寄存器 */
    /* 使用SPI1 */
    /* 设置SPCON1 */
    /** 
     * bit6-5: 00
     * bit4: 1
     * bit3: 1
     * bit2: 0
     * bit1: 0
     * bit0: 0
     */
    SPCON1 = (1 << 4) | (1 << 3);

    /* 设置SPI1的波特率 */
    /* SPI OLED接受最大的频率是100ns(10Mhz) */
    /* SPI FLASH 支持80Mhz~104Mhz */
    /** 
     * 10Mhz = PCLK / 2 / (Prescaler value + 1)
     * PCLK = 50Mhz
     * Prescaler value = 1.5
     * SPPRE1 = 2;
     */
    SPPRE1 = 2;

}

int spi_control_recv(void)
{
    /**
     * Rx data(1): SPCONn's TAGD bit disable = normal mode
     * write 0xFF to SPTDATn, then confirm REDY to set, 
     * and then read data from Read Buffer
     */
    SPTDAT1 = 0xff;
    while (!(SPSTA1 & 1));
    return SPRDAT1;
}

void spi_control_send(unsigned char val)
{
    /* 根据SPI发送数据的文档提示 */
    /** 
     * Tx data Check the status of Transfer Ready flag (REDY=1),
     * and then write data to SPTDATn.
     */
    while (!(SPSTA1 & 1));
    
    /* 发送数据 */
    SPTDAT1 = val;
}

int spi_control_gpio_init(void)
{
    /* 配置GPIO管脚模拟SPI */
    
    /* SPICLK GPG7 SPICLK1 */
    /* SPIMOSI GPG6 SPIMOSI1 */
    /* SPIMISO GPG5 SPIMISO1 */
    /* OLED_DC GPG4 输出引脚 */
    /* FLASH_CSn GPG2 输出引脚 */

    GPGCON &= ~((3 << 14) | (3 << 12) | (3 << 10) | (3 << 8) | (3 << 4));
    GPGCON |= (3 << 14) | (3 << 12) | (3 << 10) | (1 << 8) | (1 << 4);

    /* OLED_CSn GPF1 输出引脚 */
    GPFCON &= ~(3 << 2);
    GPFCON |= (1 << 2);

    /* 初始化SPI的所有引脚的值 */
    GPGDAT |= (1 << 4) | (1 << 2);

    GPFDAT |= (1 << 1);

    return 0;
}

