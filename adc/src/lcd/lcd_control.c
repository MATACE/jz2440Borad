#include "lcd_control.h"
#include "s3c2440_lcd_control.h"

#define LCD_CONTROL_NUM 32

static P_LCD_CONTROL_OP lcd_control_ops_array[LCD_CONTROL_NUM] = {};

int lcd_control_init(char *name, struct S_LCD_PARAMS * lcd_params, P_LCD_CONTROL_OP *select)
{
    for (unsigned int i = 0; i < LCD_CONTROL_NUM; i++) {
        /* 检查是否为空 名字比对 */
        if (lcd_control_ops_array[i] && (strcmp(name, lcd_control_ops_array[i]->name) == 0)) {
            lcd_control_ops_array[i]->init(lcd_params);
            /* 回传递出参数 */
            *select = lcd_control_ops_array[i];
            return 1;
        }
        return -1;
    }
    
    return -1;
}

int regist_lcd_controller(P_LCD_CONTROL_OP ops)
{
    for (unsigned int i = 0; i < LCD_CONTROL_NUM; i++) {
        /* 第一个不为空的指针存放 */
        if (!lcd_control_ops_array[i]) {
            lcd_control_ops_array[i] = ops;
            return 1;
        }
    }
    return 0;
}

void regist_all_lcd_controller(void)
{
    s3c2440_control_add();
    return;
}



