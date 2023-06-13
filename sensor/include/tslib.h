#ifndef TSLIB_H
#define TSLIB_H

#include "lcd.h"
#include "geometry.h"
#include "touch_screen.h"

void get_tslib_point_data(int lcd_x, int lcd_y, int *dx, int *dy);
int touch_screen_swap(int ts_ax, int ts_ay, int ts_bx, int ts_by);
void swap_xy_val(int *x, int *y);
void ts_rectify(void);
int ts_to_lcd_read(int *lcd_x, int *lcd_y, int *lcd_press_st);


#endif // TSLIB_H


