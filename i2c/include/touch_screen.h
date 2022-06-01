#ifndef TOUCH_SCREEN_H
#define TOUCH_SCREEN_H

#include "my_printf.h"
#include "s3c2440_soc.h"
#include "interrupt.h"
#include "timercount.h"

void tc_irq_handle(void);
void touch_screen_irq_handle(unsigned int irq_no);
void touch_screen_control_init(void);
void touch_screen_adc_init(void);
void touch_screen_wait_down_mode(void);
void touch_screen_wait_up_mode(void);
void touch_screen_init(void);
void auto_mode_get_x_y_pos(void);
void adc_x_y_irq_handle(void);
void touch_screen_timer_irq_handle(void);
void get_pen_x_y(int x, int y, int press_st);
void read_pen_pot(int *x, int *y, int *press_st);

#endif // TOUCH_SCREEN_H

