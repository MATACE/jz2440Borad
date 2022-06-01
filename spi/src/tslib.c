#include "tslib.h"

static double kx = 0;
static double ky = 0;

static int kx_ts_xc = 0;
static int kx_lcd_xc = 0;
static int ky_ts_yc = 0;
static int ky_lcd_yc = 0;

static int swap_xy_flag = 0;

static unsigned int xres = 0;
static unsigned int yres = 0;
static unsigned int bpp = 0;
static unsigned int addr_base = 0;

int get_lcd_x(int ts_x)
{
    return kx * (ts_x - kx_ts_xc) + kx_lcd_xc;
}

int get_lcd_y(int ts_y)
{
    return ky * (ts_y - ky_ts_yc) + ky_lcd_yc;
}

void get_tslib_point_data(int lcd_x, int lcd_y, int *dx, int *dy)
{
    int x = 0;
    int y = 0;
    int press_st = 0;
    int x_sum = 0;
    int y_sum = 0;
    int cnt = 0;

    /* 显示点击的十字架 */
    adjust_cross(lcd_x, lcd_y, 0xffffff);

    /* 松开事件上报了两次,忽略其中的一次 */
    do {
        read_pen_pot(&x, &y, &press_st);
    } while (press_st == 0);
    
    do {
        /* 求平均值 */
        if (cnt < 128) {
            x_sum += x;
            y_sum += y;
            cnt++;
        }
        
        read_pen_pot(&x, &y, &press_st);
        printf("get raw data: x = %08d y = %08d cnt = %d\n\r", x, y, cnt);
    } while (press_st);
    
    *dx = x_sum / cnt;
    *dy = y_sum / cnt;
    cnt = 0;
    x_sum = 0;
    y_sum = 0;

    printf("return data: x = %08d y = %08d\n\r", *dx, *dy);

    /* 消除点击后的十字图形 */
    adjust_cross(lcd_x, lcd_y, 0);

    /* 松开后才返回绘制下一个图形 */

    
    return;
}

/**
 * @brief 当X轴和Y轴不反转或者不是从小到大时，进行反转处理
 * 
 * @return int 
 */
int touch_screen_swap(int ts_ax, int ts_ay, int ts_bx, int ts_by)
{
    /* 计算A点和B点之间的数据 */
    int len_x = ts_bx - ts_ax;
    int len_y = ts_by - ts_ay;

    /* 数据方向反了,转换为正值 */
    if (len_x < 0) {
        len_x = 0 - len_x;
    }
    if (len_y < 0) {
        len_y = 0 - len_y;
    }
    
    /* X轴上的数据大于Y轴, 说明X轴在变化 */
    if (len_x > len_y) {
        return 0;
    } else {
        /* X轴上的数据小于Y轴说明, 说明Y轴在变化, X轴和Y轴反向 */
        return 1;
    }

}

/**
 * @brief X轴和Y轴的数据反转,需要交换X和Y轴的值
 * 
 * @param x 
 * @param y 
 */
void swap_xy_val(int *x, int *y)
{
    int tmp = 0;
    tmp = *x;
    *x = *y;
    *y = tmp;
    return;
}

/*
----------------------------
|                          |
|  +(A)              (B)+  |
|                          |
|                          |
|                          |
|            +(E)          |
|                          |
|                          |
|                          |
|  +(D)              (C)+  |
|                          |
----------------------------
*/

void ts_rectify(void)
{

    /* 十字架的数据 */
    int a_ts_x = 0;
    int a_ts_y = 0;

    int b_ts_x = 0;
    int b_ts_y = 0;

    int c_ts_x = 0;
    int c_ts_y = 0;

    int d_ts_x = 0;
    int d_ts_y = 0;

    int e_ts_x = 0;
    int e_ts_y = 0;

    /* 校准公式计算 */
    /* X轴方向 */
    /**
     * Kx = (s1 + s2) / (s'1 + s'2) = (2s) / (s'1 + s'2)
     * 
     */
    int ts_s1 = 0;
    int ts_s2 = 0;
    int lcd_s = 0;

    /* Y轴方向 */
    /**
     * Ky = (d1 + d2) / (d'1 + d'2) = (2d) / (d'1 + d'2)
     * 
     */
    int ts_d1 = 0;
    int ts_d2 = 0;
    int lcd_d = 0;

    /* 根据原点值通过直线公式计算 */
    /** 根据计算方式
     * x LCD的x
     * y LCD的y
     * x' LCD触摸屏的x'
     * y' LCD触摸屏的y'
     * 
     * x = (xb - xa)/(x'b - x'a)*(x' - x'a)+xa
     * 用E点作为原点来获取计算的值
     * x = kx*(x' - x'e)+xe
     * 
     * y = (yc - yd)/(y'c - y'd)*(y' - y'd)+yd
     * 用E点作为原点来获取计算的值
     * y = ky*(y' - y'e)+ye
     */


    /* 获取硬件的参数 */
    get_lcd_params(&xres, &yres, &bpp, &addr_base);
    /* 在ABCDE位置显示矫正的十字架,点击获取触摸屏的值 */
    /**
     * A(50, 50)
     * B(xres - 50, 50)
     * C(xres - 50, yres - 50)
     * D(50, yres - 50)
     * E(xres/2, yres/2)
     * 
     */

    /* 显示每个点的十字架 */
    get_tslib_point_data(50, 50, &a_ts_x, &a_ts_y);
    get_tslib_point_data(xres - 50, 50, &b_ts_x, &b_ts_y);
    get_tslib_point_data(xres - 50, yres - 50, &c_ts_x, &c_ts_y);
    get_tslib_point_data(50, yres - 50, &d_ts_x, &d_ts_y);
    get_tslib_point_data(xres/2, yres/2, &e_ts_x, &e_ts_y);

    /* 判断触摸屏的X轴和Y轴是否是正常的或者反转 */
    swap_xy_flag = touch_screen_swap(a_ts_x, a_ts_y, b_ts_x, b_ts_y);
    /* X轴和Y轴的数据反转时,需要交换X和Y的值 */
    if (swap_xy_flag) {
        swap_xy_val(&a_ts_x, &a_ts_y);
        swap_xy_val(&b_ts_x, &b_ts_y);
        swap_xy_val(&c_ts_x, &c_ts_y);
        swap_xy_val(&d_ts_x, &d_ts_y);
        swap_xy_val(&e_ts_x, &e_ts_y);
    }
    
    /* 确定公式参数并保存 */
    /* 获取s'1和s'2 */
    ts_s1 = b_ts_x - a_ts_x;
    ts_s2 = c_ts_x - d_ts_x;
    /* 获取s */
    lcd_s = xres - 50 - 50;

    /* 获取d'1和d'2 */
    ts_d1 = d_ts_y - a_ts_y;
    ts_d2 = c_ts_y - b_ts_y;

    /* 获取d */
    lcd_d = yres - 50 - 50;

    /* 计算出X轴和Y轴的斜率kx和ky */
    kx = ((double)(2 * lcd_s)) / (ts_s1 + ts_s2);
    ky = ((double)(2 * lcd_d)) / (ts_d1 + ts_d2);

    /* 获取原点的触摸屏坐标和LCD坐标 */
    kx_ts_xc = e_ts_x;
    kx_lcd_xc = xres / 2;
    ky_ts_yc = e_ts_y;
    ky_lcd_yc = yres / 2;

    printf("A x = %08d, y = %08d\n\r", get_lcd_x(a_ts_x), get_lcd_y(a_ts_y));
    printf("B x = %08d, y = %08d\n\r", get_lcd_x(b_ts_x), get_lcd_y(b_ts_y));
    printf("C x = %08d, y = %08d\n\r", get_lcd_x(c_ts_x), get_lcd_y(c_ts_y));
    printf("D x = %08d, y = %08d\n\r", get_lcd_x(d_ts_x), get_lcd_y(d_ts_y));
    printf("E x = %08d, y = %08d\n\r", get_lcd_x(e_ts_x), get_lcd_y(e_ts_y));

}

int ts_to_lcd_read(int *lcd_x, int *lcd_y, int *lcd_press_st)
{
    /* 根据X轴和Y轴的公式计算出X和Y触摸的数据 */
    /* 获取当前触摸屏的数据 */
    int ts_x = 0;
    int ts_y = 0;
    int press_st = 0;
    int tmp_x = 0;
    int tmp_y = 0;

    read_pen_pot(&ts_x, &ts_y, &press_st);
    /* 如果X轴和Y轴反转,交换数据 */
    if (swap_xy_flag) {
        swap_xy_val(&ts_x, &ts_y);
    }

    /* 用公式计算出LCD的X值和Y值 */
    tmp_x = kx * (ts_x - kx_ts_xc) + kx_lcd_xc;
    tmp_y = ky * (ts_y - ky_ts_yc) + ky_lcd_yc;

    if (tmp_x < 0 || tmp_x >= xres || tmp_y < 0 || tmp_y >= yres) {
        return -1;
    }

    *lcd_x = tmp_x;
    *lcd_y = tmp_y;
    *lcd_press_st = press_st;
    return 0;
}



