#include "lcd_menu.h"
#include "framebuffer.h"
#include "geometry.h"
#include "font.h"

#include "adc.h"

void lcd_menu(void)
{
    unsigned char ch = ' ';

    /* LCD init */
    /* 注册所有的LCD */
    regist_all_lcd();
    
    /* 初始化对应的LCD */
    lcd_init("lcd_4.3");

    /* 使能LCD */
    lcd_enable();

    /* 清除framebuf */
    fb_buf_clear();
    
    while (1) {
        /* 输出菜单 */
        printf("[t] Test rgb color lcd\n\r");
        printf("[l] Draw line\n\r");
        printf("[f] Show the font text\n\r");
        printf("[a] Read Nand flash\n\r");
        printf("[q] quit\n\r");
        printf("Enter selection: ");

        /* 获取字符 */
        ch = getchar();
        printf("%c\n\r", ch);

        switch (ch) {
            case 't':
                lcd_test();
            break;
            case 'l':
                lcd_draw();
            break;
            case 'f':
                font_text();
            break;
            case 'a':
                aint0_adc_test();
            break;
            case 'q':
                return;
            break;
            default:
                printf("Enter secletion err\n\r");
            break;
        }
    }
    return;
}

void lcd_test(void)
{
    unsigned int xres = 0;
    unsigned int yres = 0;
    unsigned int bpp = 0;
    unsigned int addr_base = 0;
    
    unsigned int *frame_buf_addr = NULL;
    unsigned short *fb_16bpp_addr = NULL;

    // /* 注册所有的LCD */
    // regist_all_lcd();
    
    // /* 初始化对应的LCD */
    // lcd_init("lcd_4.3");

    // /* 使能LCD */
    // lcd_enable();
       
    /* 清除数据 */
    fb_buf_clear();
    /* 往framebuf的地址写入数据 */
    get_lcd_params(&xres, &yres, &bpp, &addr_base);

    if (bpp == 16) {
        /* 1个字节代表一个内存格子 */
        /* GREEN */
        fb_16bpp_addr = (unsigned short *)addr_base;
        for (unsigned int x = 0; x < xres; x++) {
            for (unsigned int y = 0; y < yres; y++) {
                /* 写入内存数据 */
                *fb_16bpp_addr++ = 0x7E0;
            }
        }

        /* RED */
        fb_16bpp_addr = (unsigned short *)addr_base;
        for (unsigned int x = 0; x < xres; x++) {
            for (unsigned int y = 0; y < yres; y++) {
                /* 写入内存数据 */
                *fb_16bpp_addr++ = 0xF800;
            }
        }

        /* BLUE */
        fb_16bpp_addr = (unsigned short *)addr_base;
        for (unsigned int x = 0; x < xres; x++) {
            for (unsigned int y = 0; y < yres; y++) {
                /* 写入内存数据 */
                *fb_16bpp_addr++ = 0x1F;
            }
        }
    } else if (bpp == 32) {
        /* 1个字节代表一个内存格子 */
        /* RED */
        frame_buf_addr = (unsigned int *)addr_base;
        for (unsigned int x = 0; x < xres; x++) {
            for (unsigned int y = 0; y < yres; y++) {
                /* 写入内存数据 */
                *frame_buf_addr++ = 0xFF0000;
            }
        }
        /* GREEN */
        frame_buf_addr = (unsigned int *)addr_base;
        for (unsigned int x = 0; x < xres; x++) {
            for (unsigned int y = 0; y < yres; y++) {
                /* 写入内存数据 */
                *frame_buf_addr++ = 0x00FF00;
            }
        }
        /* BLUE */
        frame_buf_addr = (unsigned int *)addr_base;
        for (unsigned int x = 0; x < xres; x++) {
            for (unsigned int y = 0; y < yres; y++) {
                /* 写入内存数据 */
                *frame_buf_addr++ = 0x0000FF;
            }
        }
    }

    /* 关闭LCD */
    // lcd_disalbe();

    return;
}

void lcd_draw(void)
{
    unsigned int xres = 0;
    unsigned int yres = 0;
    unsigned int bpp = 0;
    unsigned int addr_base = 0;

    get_lcd_params(&xres, &yres, &bpp, &addr_base);

    /* 清除数据 */
    fb_buf_clear();

    for (unsigned int i = 31; i < 250; i++) {
        paint_pot(i, 32, 0xff0000);
    }

    for (unsigned int i = 31; i < 250; i++) {
        paint_pot(31, i, 0x00ff00);
    }

    for (unsigned int i = 31; i < 250; i++) {
        paint_pot(250, i, 0x0000ff);
    }

    // for (unsigned int i = 0; i < 250; i++) {
    //     paint_pot(0, i, 0x0000ff);
    // }

    /* 画线 */
    draw_line(0, 0, xres - 1, 0, 0xff0000);
    draw_line(xres-1, 0, xres - 1, yres - 1, 0xffff00);
    draw_line(0, yres - 1, xres - 1, yres - 1, 0xff00ff);
    draw_line(0, 0, 0, yres - 1, 0xffee00);
    draw_line(0, 0, xres - 1, yres - 1, 0xffee);
    draw_line(xres - 1, 0, 0, yres - 1, 0xff33aa);
    
    /* 画圆 */
    draw_circle(xres / 2, yres / 2, 32, 0x33aaff);

    return;
}

void font_text(void)
{
    /* 清除数据 */
    fb_buf_clear();
    /* 初始化显示字符 */
    font_init();

    print_font_char(32, 32, 'A', 0xff00);

    print_font_string(0, 0, "Hello World\n\r Good Work", 0xff0000);

    return;
}

void aint0_adc_test(void)
{
    unsigned int m = 0;
    unsigned int n = 0;
    double vol = 0;
    unsigned int val = 0;

    /* ADC测试 */
    adc_init();
    
    while (1) {
        val = get_ain0_adc_val();
        vol = (double)val / 1023 * 3.3;
        m = (int)vol;
        n = (vol - m) * 1000;
        printf("ADC: %d.%02dv\r", m, n);
    }
    

}
