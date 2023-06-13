#include "font.h"
#include "lcd.h"
#include "framebuffer.h"

static unsigned int fb_xres;
static unsigned int fb_yres;
static unsigned int fb_bpp;
static unsigned int fb_base_addr;

void font_init(void)
{
    get_lcd_params(&fb_xres, &fb_yres, &fb_bpp, &fb_base_addr);
    return;
}

void print_font_char(unsigned int x, unsigned int y, unsigned char ch, unsigned int color)
{
    /* 根据字符的x和y坐标 8*16画字符 */
    /* 根据asicc获取字符位置的数据 */
    unsigned char *ch_addr = &fontdata_8x16[ch*16];
    unsigned int bit = 7;

    /* y轴16个数据 */
    for (unsigned int j = y; j < y + 16; j++) {
        /* 重置每一行数据 */
        for (unsigned int i = x; i < x + 7; i++) {
            /* 取出每个字节的bit位 */
            if ((*ch_addr >> bit) & 1) {
                /* 显示当前的点 */
                paint_pot(i, j, color);
            } else {
                /* 不显示当前的点 */
                paint_pot(i, j, 0x0);
            }
            bit--;
        }
        /* 获取下一行的显示数据 */
        ch_addr++;
        bit = 7;
    }

    return;
}

void print_font_string(unsigned int x, unsigned int y, const char *buf, unsigned int color)
{
    if (x >= 480 || y >= 272 || x < 0 || y < 0) {
        return;
    }

    /* 显示字符串 */
    while (*buf) {
        if (*buf == '\n') {
            y += 16;
        } else if (*buf == '\r') {
            x = 0;
        } else {
            /* 逐行显示字符 */
            print_font_char(x, y, *buf, color);
            /* x和y值变化 */
            x += 8;
            if (x >= fb_xres) {
                x = 0;
                y += 16;
            }
        }
        buf++;
    }

    return;
}


