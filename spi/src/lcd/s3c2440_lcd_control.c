#include "s3c2440_lcd_control.h"

#define HCLK 100

void s3c2440_lcd_gpio_init(void)
{
    /* LED背光KEYBOARD GPB0*/
    GPBCON = GPBCON & (~(3 << 0));
    GPBCON = GPBCON | (1 << 0);
    // /* LEND GPC0*/
    // GPCCON = GPCCON & (~(3 << 0));
    // GPCCON = GPCCON | (2 << 0);
    // /* VCLK时钟 GPC1 */
    // GPCCON = GPCCON & (~(3 << 2));
    // GPCCON = GPCCON | (2 << 2);
    // /* VLINE GPC2 */
    // GPCCON = GPCCON & (~(3 << 8));
    // GPCCON = GPCCON | (2 << 8);
    // /* VFRAME GPC3 */
    // GPCCON = GPCCON & (~(3 << 6));
    // GPCCON = GPCCON | (2 << 6);
    // /* VM GPC4 */
    // GPCCON = GPCCON & (~(3 << 8));
    // GPCCON = GPCCON | (2 << 8);
    // /* GPC5~GPC10 */
    // GPCCON = GPCCON & (~((3 << 10) | (3 << 12) | (3 << 14) | (3 << 16) | (3 << 18) | (3 << 20)));
    // GPCCON = GPCCON | ((2 << 10) | (2 << 12) | (2 << 14) | (2 << 16) | (2 << 18) | (2 << 20));
    /* LCD_PWRDN GPG4 */
    GPGCON = GPGCON & (~(3 << 8));
    GPGCON = GPGCON | (3 << 8);
    // /* VD3~VD7 GPC11~GPC15*/
    // GPCCON = GPCCON & (~((3 << 30) | (3 << 28) | (3 << 26) | (3 << 24) | (3 << 22)));
    // GPCCON = GPCCON | ((2 << 30) | (2 << 28) | (2 << 26) | (2 << 24) | (2 << 22));
    /* VD15~VD19 GPD7~GPD11*/
    // GPDCON = GPDCON & (~((3 << 22) | (3 << 20) | (3 << 18) | (3 << 16) | (3 << 14)));
    // GPDCON = GPDCON | ((2 << 22) | (2 << 20) | (2 << 18) | (2 << 16) | (2 << 14));

    /* LCD初始化 */ 
    GPCCON = 0xAAAAAAAA;
    GPDCON = 0xAAAAAAAA;
}

void s3c2440_lcd_init(struct S_LCD_PARAMS *lcd_params)
{
    /* 初始化GPIO管脚 */
    s3c2440_lcd_gpio_init();

    /* 设置s3c2440的LCD控制器 */

    unsigned int display_place = 0;
    unsigned int addr = 0;

    /* LCDCON1 bit17-8:CLKVAL TFT: VCLK = HCLK / [(CLKVAL+1) x 2] */
    /** HCLK = 100Mhz  vclkval = HCLK/VCLK/2-1 VCLK范围:5~10Mhz
     *  VCLK手册给出9Mhz vclkval = 100/9/2-1=4.555 取整数vclkval = 5
     */
    /* 设置时钟 bit17:bit8-CLKVAL*/
    // unsigned int vclkval = HCLK/lcd_params->times_set.vclk/2-1+1;
    unsigned int vclkval = 5;
    /* 设置PNRMODE bit6:bit5-11 */
    /* 设置BPPMODE 8bpp 16bpp 32bpp */
    unsigned int bppmode = lcd_params->display_set.bpp == 8 ? 0xb :\
                           lcd_params->display_set.bpp == 16 ? 0xc:\
                           0xd;
    /* 设置ENVID 控制LCD输出*/
    LCDCON1 = (vclkval << 8) | (3 << 5) | (bppmode << 1);

    /* VBPD Vertical back porch: tvp-1         bit31:bit24 */
    /* LINEVAL Vertical y行: yres-1             bit23:bit14 */
    /* VFPD Vertical front porch:tvf-1         bit13:bit6 */
    /* VSPW Vertical sync pulse width: tvp-1   bit5:bit0 */
    LCDCON2 = ((lcd_params->times_set.tvp-1) << 24) |\
              ((lcd_params->display_set.yres-1) << 14) |\
              ((lcd_params->times_set.tvf-1) << 6) |\
              ((lcd_params->times_set.tvp-1) << 0);

    /* HBPD Horizontal back porch: thb-1  bit25:bit19 */
    /* HOZVAL Horizontal x列: xres-1       bit18:bit8 */
    /* HFPD front porch:thf-1             bit7:bit0 */
    LCDCON3 = ((lcd_params->times_set.thb-1) << 19) |\
              ((lcd_params->display_set.xres-1) << 8) |\
              ((lcd_params->times_set.thf-1) << 0);

    /* HSPW Horizontal sync pulse width: thd-1  bit7:bit0 */
    LCDCON4 = (lcd_params->times_set.thp-1) << 0;

    /** 24 bpp?         bit12
     *  FRM565?         bit11
     *  VCLK?           bit10
     *  HSYNC?          bit9
     *  VSYNC?          bit8
     *  VD?             bit7
     *  VDEN?           bit6
     *  PWREN?          bit5
     *  LEND?           bit4
     *  LCD_PWREN?      bit3
     *  LEND?           bit2
     *  Byte swap?      bit1
     *  Half-Word swap? bit0
    */

    /** 计算4字节存放数据的高低位置
     *  32bpp: BSWP = 0, HWSWP = 0, BPP24BL = 0 : bit23:bit0
     *  16bpp: BSWP = 0, HWSWP = 1 : bit31:bit16---bit15:bit0
     *  8bpp:  BSWP = 1, HWSWP = 0 : bit31:bit24--bit23:bit16--bit15:bit8--bit7:bit0
     */
    display_place = lcd_params->display_set.bpp == 32 ? 0 :\
                    lcd_params->display_set.bpp == 16 ? 1 :\
                    (1 << 1);

    LCDCON5 = display_place |\
              (1 << 11) | (lcd_params->pins_set.vclk << 10) | (lcd_params->pins_set.hsync << 9) |\
              (lcd_params->pins_set.vsync << 8) | (lcd_params->pins_set.rgb << 7) |\
              (lcd_params->pins_set.de << 6) |(lcd_params->pins_set.pwren << 5);
    
    /* frame buffer */
    /** bit29:bit21 of address bit30:bit22 
     *  bit20:bit0 of address bit21:bit1 
    */

    /* frame buffer起始地址 */
    addr = lcd_params->fb_addr_base & (~(1 << 31));
    LCDSADDR1 = (addr >> 1);

    /* frame buffer结束地址 */
    /* 计算参考例子 */
    /* bit29:bit0 of address bit21:bit1 */

    addr = lcd_params->fb_addr_base + lcd_params->display_set.xres * lcd_params->display_set.yres *\
           lcd_params->display_set.bpp / 8;
    addr >>= 1;
    addr &= 0x1fffff;
    LCDSADDR2 = addr;

}

void s3c2440_lcd_enable(void)
{
    /* LED背光KEYBOARD GPB0*/
    GPBDAT = GPBDAT | (1 << 0);
    /* pwren 给LCD提供电源 */
    LCDCON5 |= (1 << 3);
    /* LCDCON1 bit0 :设置LCD控制器是否输出信号 */
    LCDCON1 |= (1 << 0);
}

void s3c2440_lcd_disable(void)
{
    /* LED背光KEYBOARD GPB0*/
    GPBDAT = GPBDAT & (~(1 << 0));
    /* pwren 给LCD提供电源 */
    LCDCON5 &= ~(1 << 3);
    /* LCDCON1 bit0 :设置LCD控制器是否输出信号 */
    LCDCON1 &= ~(1 << 0);
}

static LCD_CONTROL_OP s3c2440_lcd_control_op = {
    .name = "lcd_4.3",
    .init = s3c2440_lcd_init,
    .enable = s3c2440_lcd_enable,
    .disable = s3c2440_lcd_disable,
};

void s3c2440_control_add(void)
{
    regist_lcd_controller(&s3c2440_lcd_control_op);
    return;
}

