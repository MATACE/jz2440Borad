#include "gpio_spi.h"

int gpio_spi_init(void)
{
    /* 配置GPIO管脚模拟SPI */
    
    /* SPICLK GPG7 输出引脚 */
    /* SPIMOSI GPG6 输出引脚 */
    /* SPIMISO GPG5 输入引脚 */
    /* OLED_DC GPG4 输出引脚 */
    /* FLASH_CSn GPG2 输出引脚 */

    GPGCON &= ~((3 << 14) | (3 << 12) | (3 << 10) | (3 << 8) | (3 << 4));
    GPGCON |= (1 << 14) | (1 << 12) | (1 << 8) | (1 << 4);

    /* OLED_CSn GPF1 输出引脚 */
    GPFCON &= ~(3 << 2);
    GPFCON |= (1 << 2);

    /* 初始化SPI的所有引脚的值 */
    GPGDAT |= (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 2);
    // GPGDAT = (1 << 2);

    GPFDAT |= (1 << 1);

    return 0;
}

void spi_set_sclk(PIN_SET val)
{
    if (val == P_SET) {
        GPGDAT |= (1 << 7);
    } else if (val == P_UNSET) {
        GPGDAT &= ~(1 << 7);
    }
}

void spi_set_do(char val) 
{
    if (val) {
        GPGDAT |= (1 << 6);
    } else {
        GPGDAT &= ~(1 << 6);
    }
}

int spi_get_di(void)
{
    if (GPGDAT & (1 << 5)) {
        /* 获取高电平 */
        return 1;
    } else {
        /* 获取低电平 */
        return 0;
    }
}

void spi_send_byte(unsigned char val)
{
    /* 从最高位发送每个bit数据 */
    for (unsigned int i = 0; i < 8; i++) {
        /* SPI数据在上升沿有效 */
        /* 拉低spi的sclk */
        spi_set_sclk(P_UNSET);
        /* 设置DO的数据 */
        spi_set_do(val & 0x80);
        /* 拉高spi的sclk */
        spi_set_sclk(P_SET);
        val <<= 1;
    }
}

int spi_recv_byte(void)
{
    unsigned char val = 0;
    /* 从最高位的数据接受 */
    for (unsigned int i = 0; i < 8; i++) {
        val <<= 1;
        /* SPI数据在上升沿有效 */
        /* 拉低spi的sclk */
        spi_set_sclk(P_UNSET);
        /* 设置Di的数据 */
        val = val | spi_get_di();
        /* 拉高spi的sclk */
        spi_set_sclk(P_SET);
    }
    return val;
}

void spi_init()
{
    /* 初始化管脚 */
    gpio_spi_init();
}


