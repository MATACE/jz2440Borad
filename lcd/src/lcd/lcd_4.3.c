#include "lcd_4.3.h"

#define LCD_FB_BASE 0x33c00000

static LCD_PARAMS lcd_4_3_params = {
    .name = "lcd_4.3",
    /* 看LCD原理图确定有效电平, NORMAL/INVERT代表芯片的默认输出电平*/
    .display_set = {
        .xres = 480,
        .yres = 272,
        .bpp = 32,
    },
    .pins_set = {
        .de = NORMAL,
        .pwren = NORMAL,
        .vclk = NORMAL,
        .rgb = NORMAL,
        .hsync = INVERT, /* 低脉冲有效 */
        .vsync= INVERT,  /* 低脉冲有效 */
    },
    .times_set = {
        /* 水平方向 */
        .thf = 2,
        .thp = 41,
        .thb = 2,

        /* 垂直方向 */
        .tvb = 2,
        .tvp = 10,
        .tvf = 2,

        .vclk = 9 /*Mhz*/
    },
    .fb_addr_base = LCD_FB_BASE,
};

void s3c2440_params_add(void)
{
    regist_lcd_params(&lcd_4_3_params);
    return;
}





