#include "framebuffer.h"
#include "lcd.h"

static unsigned int fb_xres;
static unsigned int fb_yres;
static unsigned int fb_bpp;
static unsigned int fb_base_addr;

unsigned short get_16bpp_rgb(const unsigned int rgb)
{
    /* 获取16bpp的rgb颜色 */
    /* 0x00RRGGBB RR-8 GG-8 BB-8 */
    /* 0xRGB R-5 G-6 B-5 */
    unsigned int r = (rgb >> 16) & 0x1f;
    unsigned int g = (rgb >> 8) & 0x3f;
    unsigned int b = rgb & 0x1f;

    return ((r << 11) | (g << 5) | b);
}

/**
 * @brief Paint the lcd pot.
 * 
 * @param x 
 * @param y 
 * @param rgb 32bit format 0x00RRGGBB RR-8 GG-8 BB-8
 */
void paint_pot(const unsigned int x, const unsigned int y, const unsigned int rgb) 
{

    unsigned char *pot_addr = 0;
    unsigned short *pot_16_addr = 0;
    unsigned int *pot_32_addr = 0;

    if ((x >= 480) | (y >= 272)) {
       return;
    }
    /** 计算内存坐标值
     *  pot_addr = (unsigned char *)base_addr + (x * xres * bpp / 8) + y * bpp / 8;
    */

    get_lcd_params(&fb_xres, &fb_yres, &fb_bpp, &fb_base_addr);
    pot_addr = (unsigned char *)fb_base_addr + (y * fb_xres * fb_bpp / 8) + x * fb_bpp / 8;
    
    switch (fb_bpp) {
        case 8:
        break;
        case 16:
            pot_16_addr = (unsigned short *)pot_addr;
            *pot_16_addr = get_16bpp_rgb(rgb);
        break;
        case 32:
            pot_32_addr = (unsigned int *)pot_addr;
            *pot_32_addr = rgb;
        break;
        default:
        break;
    }

    return;
}

void fb_buf_clear(void)
{
    unsigned char *pot_addr = 0;
    unsigned short *pot_16_addr = 0;
    unsigned int *pot_32_addr = 0;

    get_lcd_params(&fb_xres, &fb_yres, &fb_bpp, &fb_base_addr);
    pot_addr = (unsigned char *)fb_base_addr;

    /* bpp不同 framebuf的结束地址不同 */
    switch (fb_bpp) {
        case 8:
        break;
        case 16:
            pot_16_addr = (unsigned short *)pot_addr;
            for (unsigned int x = 0; x < fb_xres; x++) {
                for (unsigned int y = 0; y < fb_yres; y++) {
                    /* 写入内存数据 */
                    *pot_16_addr++ = 0x0;
                }
            }
        break;
        case 32:
            pot_32_addr = (unsigned int *)pot_addr;
            for (unsigned int x = 0; x < fb_xres; x++) {
                for (unsigned int y = 0; y < fb_yres; y++) {
                    /* 写入内存数据 */
                    *pot_32_addr++ = 0x0;
                }
            }
        break;
        default:
        break;
    }

    return;

}


