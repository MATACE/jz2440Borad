#ifndef LCD_CONTROL_H
#define LCD_CONTROL_H

#include "lcd.h"
#include "s3c2440_soc.h"
#include "string_utils.h"
#include "my_printf.h"

extern struct S_LCD_PARAMS;

typedef struct {
    char *name;
    void (*init)(struct S_LCD_PARAMS *lcd_params);
    void (*enable)(void);
    void (*disable)(void);
} LCD_CONTROL_OP, *P_LCD_CONTROL_OP;

int lcd_control_init(char *name, struct S_LCD_PARAMS * lcd_params, P_LCD_CONTROL_OP *select);
int regist_lcd_controller(P_LCD_CONTROL_OP ops);

#endif // LCD_CONTROL_H

