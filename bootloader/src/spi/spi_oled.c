#include "spi_oled.h"

void oled_set_cs(CHIP val) 
{
    /* pin pull low: enable */
    if (val == CHIP_D) {
        GPFDAT |= 1 << 1;
    } else if (val == CHIP_E) {
        GPFDAT &= ~(1 << 1);
    }
}

void oled_set_dc(OLED_MODE val)
{
    /* D/C 1:data mode, 0:cmd mode */
    if (val == DATA_M) {
        GPGDAT |= (1 << 4);
    } else if (val == CMD_M) {
        GPGDAT &= ~(1 << 4);
    }
}

int oled_write_data(unsigned char data)
{
    /* 设置oled的D/C引脚 */
    oled_set_dc(DATA_M);

    /* 设置oled的CS引脚选中 */
    oled_set_cs(CHIP_E);

    
    if (SPI_MODE_INIT_FLAG == 0) {
        /* 从SPI中发送数据 */
        spi_send_byte(data);
    } else {
        /* 从SPI中发送数据 */
        spi_control_send(data);
    }

    /* 首先应该取消片选 */
    /* 设置oled的CS引脚选中 */
    oled_set_cs(CHIP_D);

    /* 设置oled的D/C引脚 */
    oled_set_dc(DATA_M);

    return 0;
}

int oled_write_cmd(unsigned char data)
{
    /* 设置oled的D/C引脚 */
    oled_set_dc(CMD_M);

    /* 设置oled的CS引脚选中 */
    oled_set_cs(CHIP_E);

    if (SPI_MODE_INIT_FLAG == 0) {
        /* 从SPI中发送数据 */
        spi_send_byte(data);
    } else {
        /* 从SPI中发送数据 */
        spi_control_send(data);
    }

    /* 首先应该取消片选 */
    /* 设置oled的CS引脚选中 */
    oled_set_cs(CHIP_D);

    /* 设置oled的D/C引脚 */
    oled_set_dc(DATA_M);

    return 0;
}

int oled_init(void)
{
    /* OLED初始化 */
    oled_write_cmd(0xAE); /*display off*/
    oled_write_cmd(0x00); /*set lower column address*/
    oled_write_cmd(0x10); /*set higher column address*/
    oled_write_cmd(0x40); /*set display start line*/
    oled_write_cmd(0xB0); /*set page address*/
    oled_write_cmd(0x81); /*contract control*/
    oled_write_cmd(0x66); /*128*/
    oled_write_cmd(0xA1); /*set segment remap*/
    oled_write_cmd(0xA6); /*normal / reverse*/
    oled_write_cmd(0xA8); /*multiplex ratio*/
    oled_write_cmd(0x3F); /*duty = 1/64*/
    oled_write_cmd(0xC8); /*Com scan direction*/
    oled_write_cmd(0xD3); /*set display offset*/
    oled_write_cmd(0x00);
    oled_write_cmd(0xD5); /*set osc division*/
    oled_write_cmd(0x80);
    oled_write_cmd(0xD9); /*set pre-charge period*/
    oled_write_cmd(0x1f);
    oled_write_cmd(0xDA); /*set COM pins*/
    oled_write_cmd(0x12);
    oled_write_cmd(0xdb); /*set vcomh*/
    oled_write_cmd(0x30);
    oled_write_cmd(0x8d); /*set charge pump enable*/
    oled_write_cmd(0x14);

    /* 设置page的模式 */
    oled_set_page_addr();

    /* 清除oled的屏幕 */
    oled_clear();
    // oled_full();

    oled_write_cmd(0xAF); /*display ON*/

    return 0;
}

void oled_full(void)
{
    unsigned int page = 0;
    unsigned int i = 0;
    /* 设置所有page中的数据都为0 */
    for (page = 0; page < 8; page++)
    {
        /* 发送oled的page的地址命令 */
        oled_page_pos(page, 0);
        for (i = 0; i < 128; i++) {
            /* 所有地址数据写0 */
            oled_write_data(0xff);
        }      
    }
}

void oled_clear(void)
{
    unsigned int page = 0;
    unsigned int i = 0;
    /* 设置所有page中的数据都为0 */
    for (page = 0; page < 8; page++)
    {
        /* 发送oled的page的地址命令 */
        oled_page_pos(page, 0);
        for (i = 0; i < 128; i++) {
            /* 所有地址数据写0 */
            oled_write_data(0);
        }      
    }
}

void oled_set_page_addr(void)
{
    oled_write_cmd(0x20);
    oled_write_cmd(0x02);

}

void oled_page_pos(unsigned int page, unsigned col)
{
    /* 发送oled的page的地址命令 */
    oled_write_cmd(0xB0 + page);
    
    /* 发送Lower Column Start Address */
    oled_write_cmd(col & 0xf);

    /* 发送Higher Column Start Address  */
    oled_write_cmd(0x10 + (col >> 4));

}

void oled_putchar(unsigned int page, unsigned int col, char ch)
{
    /* 在8*16上显示字符 */
    

    /* 获取到需要显示的字符的字模 */
    const unsigned char *buf = font_8X16[ch - ' '];

    /* 显示字符 */
    /* 发送第一组8字节的数据 */
    /* 发送OLED的page的地址 */
    oled_page_pos(page, col);
    for (unsigned int i = 0; i < 8; i++) {
        oled_write_data(buf[i]);
    }
    
    /* 发送第二组8字节的数据 */
    /* 发送OLED的page+1的地址 */
    oled_page_pos(page+1, col);
    for (unsigned int i = 0; i < 8; i++) {
        oled_write_data(buf[i+8]);
    }
}

void oled_print(unsigned int page, unsigned int col, char *buf)
{
    unsigned int i = 0;
    /* 在oled上显示数据 */
    while (buf[i]) {
        /* 显示当前的字符 */
        oled_putchar(page, col, buf[i]);
        /* 根据OLED的128*64切换到下一行显示 */
        col += 8;
        if (col > 127) {
            /* OLED切换到下一行显示 */
            /* 纵轴方向是16,2*8,是两个page */
            page += 2;
            /* 显示到最初始的位置 */
            col = 0;
        }
        i++;
    }
}


