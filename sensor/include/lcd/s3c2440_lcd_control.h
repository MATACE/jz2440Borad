#ifndef S3C2440_LCD_CONTROL_H
#define S3C2440_LCD_CONTROL_H

#include "lcd_control.h"

extern struct S_LCD_PARAMS;

extern void s3c2440_lcd_gpio_init(void);
extern void s3c2440_lcd_init(struct S_LCD_PARAMS *lcd_params);
extern void s3c2440_lcd_enable(void);
extern void s3c2440_lcd_disable(void);

extern void s3c2440_control_add(void);

#endif // S3C2440_LCD_CONTROL_H

