#ifndef LCD_H
#define LCD_H

#include "lcd_4.3.h"
#include "s3c2440_lcd_control.h"
#include "lcd_control.h"

enum {
    /* 有效的脉冲信号 */
    /* NORMAL:正常的极性 */
    NORMAL = 0,
    /* INVERT:反转的极性 */
    INVERT = 1,
};

typedef struct {
    /* 信号是高电平有效或低电平有效 */
    unsigned int de;    /* normal:高电平 */
    unsigned int pwren; /* normal:高电平 */
    unsigned int vclk;  /* normal:数据在下降沿 */
    unsigned int rgb;   /* normal:高电平读取数据 */
    unsigned int hsync; /* normal:高脉冲 */
    unsigned int vsync; /* normal:高脉冲 */
} PINS_POLARITY, *P_PINS_POLARITY;

typedef struct {
    /* 水平方向和垂直方向的信号脉冲 */
    /* 垂直方向 */
    unsigned int tvp; /* vysnc脉冲宽度 */
    unsigned int tvb; /* 垂直最上边back黑框脉冲宽度 */
    unsigned int tvf; /* 垂直最下边front黑框脉冲宽度 */
    /* 水平方向 */
    unsigned int thp; /* vysnc脉冲宽度 */
    unsigned int thb; /* 水平最左边back黑框脉冲宽度 */
    unsigned int thf; /* 水平最右边front黑框脉冲宽度 */

    unsigned int vclk; /* LCD的时钟频率 */

} TIMING_SEQ, *P_TIMING_SEQ;

typedef struct {
    unsigned int xres;
    unsigned int yres;
    unsigned int bpp;
} DISPLAY, *P_DISPLAY;

typedef struct S_LCD_PARAMS {
    /* LCD的名字 */
    char *name;

    /* 引脚的有效电平 */
    PINS_POLARITY pins_set;
    /* 时序 */
    TIMING_SEQ times_set;
    /* 分辨率, bpp */
    DISPLAY display_set;
    /* framebuffer地址 */
    unsigned int fb_addr_base;
} LCD_PARAMS, *P_LCD_PARAMS;

int regist_lcd_params(P_LCD_PARAMS lcd_params);
void regist_all_lcd(void);
int lcd_init(char *name);
int lcd_enable(void);
int lcd_disalbe(void);
void get_lcd_params(unsigned int *xres, unsigned int *yres, unsigned int *bpp, unsigned int *addr_base);

#endif // LCD_H

