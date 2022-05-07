#include "lcd.h"

#define LCD_TYPE_NUM 32
static P_LCD_PARAMS lcd_params_array[LCD_TYPE_NUM] = {};
static P_LCD_PARAMS p_select_lcd_params = NULL;
static P_LCD_CONTROL_OP p_select_control_op = NULL;

int regist_lcd_params(P_LCD_PARAMS lcd_params)
{
    /* 找到第一个非空的位置存放数据 */
    for (unsigned int i = 0; i < LCD_TYPE_NUM; i++) {
        if (!lcd_params_array[i]) {
            /* 注册LCD参数 */
            lcd_params_array[i] = lcd_params;
            return 1;
        }
    }

    return 0;
}

void regist_all_lcd(void)
{
    s3c2440_params_add();
    s3c2440_control_add();

    return;
}

int lcd_init(char *name)
{
    /* 根据名字初始化 */
    if (name != NULL) {
        for (unsigned int i = 0; i < LCD_TYPE_NUM; i++) {
            if (lcd_params_array[i] && strcmp(name, lcd_params_array[i]->name) == 0) {
                /* 获取当前使用的LCD的参数 */
                p_select_lcd_params = lcd_params_array[i];
                /* 调用controler初始化函数,并获取参数 */
                lcd_control_init(name, lcd_params_array[i], &p_select_control_op);
                return 1;
            }
        }
    }
    return 0;
}

int lcd_enable(void)
{
    if (p_select_control_op) {
        p_select_control_op->enable();
        return 1;
    }
    return 0;
}

int lcd_disalbe(void)
{
    if (p_select_control_op) {
        p_select_control_op->disable();
    }

    return 0;
}

void get_lcd_params(unsigned int *xres, unsigned int *yres, unsigned int *bpp, unsigned int *addr_base)
{
    *xres = p_select_lcd_params->display_set.xres;
    *yres = p_select_lcd_params->display_set.yres;
    *bpp = p_select_lcd_params->display_set.bpp;
    *addr_base = p_select_lcd_params->fb_addr_base;

    return;
}



