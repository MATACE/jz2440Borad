#include "touch_screen.h"

#define INT_ADC_BIT     (31)

#define INT_ADC_S_BIT   (10)
#define INT_TC_BIT      (9)

#define UP_STAT_BIT     (1 << 8)
#define DOWN_STAT_BIT   (0 << 8)

#define YM_ENABLE       (1 << 7)
#define YM_DISABLE      (0 << 7)

#define YP_ENABLE       (0 << 6)
#define YP_DISABLE      (1 << 6)

#define XM_ENABLE       (1 << 5)
#define XM_DISABLE      (0 << 5)

#define XP_ENABLE       (0 << 4)
#define XP_DISABLE      (1 << 4)

#define PULL_UP_ENABLE  (0 << 3)
#define PULL_UP_DISABLE (1 << 3)

#define AUTO_PST_NOR    (0 << 2)
#define AUTO_PST_AUTO   (1 << 2)

#define XY_PST_NO_OPR    (0)
#define XY_PST_WAIT      (3)

static volatile int touch_screen_press_flag = 0;

static int ts_x = 0;
static int ts_y = 0;
static volatile char pen_val_valid = 0;
static int press_state = 0;

void get_pen_x_y(int x, int y, int press_st)
{
    // printf("x = %08d, y = %08d\n\r", x, y);
    /* 获取触摸笔的x和y数据 */
    if (pen_val_valid == 0) {
        ts_x = x;
        ts_y = y;
        press_state = press_st;
        pen_val_valid = 1;
    }
}

void read_pen_pot(int *x, int *y, int *press_st)
{
    /* 提供有效的点击数据 */
    while (pen_val_valid == 0);
    *x = ts_x;
    *y = ts_y;
    *press_st = press_state;
    pen_val_valid = 0;
}


static void press_flag_enable(void)
{
    touch_screen_press_flag = 1;
    return;
}

static void press_flag_disable(void)
{
    touch_screen_press_flag = 0;
    return;
}

static int get_press_flag(void)
{
    return touch_screen_press_flag;
}

int is_in_auto_mode(void)
{
    return ADCTSC & AUTO_PST_AUTO;
}

void touch_screen_timer_irq_handle(void)
{
    /* 如果触摸屏仍被按下, 进入"自动测量模式", 启动ADC */
    /* 如果没有使能定时器读取ADC的值 */
    if (get_press_flag() == 0) {
        /* 定时器处理不做任何事 */
        return;
    }
    
    /* ADC在连续获取数据中,忽略定时器中断判断长按还是松开 */
    if (is_in_auto_mode()) {
        return;
    }

    /* 在等待中断模式下,才可以使用ADCDAT0判断是松开还是按下 */

    /* 如果松开 */
    /* 判断触摸屏按下还是松开 */
    if ((ADCDAT0 >> 15) & 1) {
        /* 松开 */
        /* 不使能定时器读取ADC的值 */
        press_flag_disable();
        /* 进入等待触摸按下模式 */
        touch_screen_wait_down_mode();
        /* 松开,设置触摸屏等待下一次按下状态,才读取触摸屏的数据 */
        get_pen_x_y(0, 0, 0);
        return;
    } else {
        /* 按下 */
        // printf("touch down\n\r");
        
        /* 启动ADC,通过自动模式获取值,计算x和y轴的数据 */
        /* 设置自动获取ADC的转换 */
        auto_mode_get_x_y_pos();

        /* 启动ADC配合自动转换 */
        ADCCON |= (1 << 0);
    }

    return;
}


void tc_irq_handle(void)
{
    /* 判断触摸屏按下还是松开 */
    if ((ADCDAT0 >> 15) & 1) {
        /* 松开 */
        // printf("touch up\n\r");
        /* 进入等待触摸按下模式 */
        touch_screen_wait_down_mode();
        /* 松开,设置触摸屏等待下一次按下状态,才读取触摸屏的数据 */
        get_pen_x_y(0, 0, 0);
        
    } else if (!((ADCDAT0 >> 15) & 1)) {
        /* 按下 */
        // printf("touch down\n\r");
        
        /* 启动ADC,通过自动模式获取值,计算x和y轴的数据 */
        /* 设置自动获取ADC的转换 */
        auto_mode_get_x_y_pos();

        /* 启动ADC配合自动转换 */
        ADCCON |= (1 << 0);
    }

    return;
}

void adc_x_y_irq_handle(void)
{
    /* 获取坐标的数值 */

    int x = ADCDAT0;
    int y = ADCDAT1;

    static int adc_cnt = 0;
    static int adc_x_sum = 0;
    static int adc_y_sum = 0;

    /*  进入ADC的自动测量模式 */

    /* 在等待中断模式下,才可以使用ADCDAT0判断是松开还是按下 */
    /* 触摸屏测试时进入等待触摸松开模式 */
    touch_screen_wait_up_mode();
 
    /* 按下才打印数值 */
    if (!((ADCDAT0 >> 15) & 1)) {
#if 0
        x &= 0x3ff;
        y &= 0x3ff;

        /* 上报数值,此时按下状态,设置触摸按下的press_state为1 */
        get_pen_x_y(x, y, 1);

        /* 启动定时器再次读取数据  */
        press_flag_enable();
#endif
        /* 通过多次求平局值减少误差 */
        /* 第一次启动ADC后,读值,连续启动N次,获取N个数据,求平均值后上报数据 */
        adc_x_sum += (x & 0x3ff);
        adc_y_sum += (y & 0x3ff);
        adc_cnt++;

        if (adc_cnt == 16) {
            adc_x_sum >>= 4;
            adc_y_sum >>= 4;
            get_pen_x_y(adc_x_sum, adc_y_sum, 1);

            /* 重置平均 */
            adc_cnt = 0;
            adc_x_sum = 0;
            adc_y_sum = 0;

            /* 进入等待触摸按下模式 */
            touch_screen_wait_up_mode();

            /* 等到平均数据后,再启动Timer定时器检测长按还是短按 */
            /* 启动定时器再次读取数据  */
            press_flag_enable();     
        } else {
            /* 再次启动ADC继续读取多个值 */
            /* 启动ADC,通过自动模式获取值,计算x和y轴的数据 */
            /* 设置自动获取ADC的转换 */
            auto_mode_get_x_y_pos();

            /* 启动ADC配合自动转换 */
            ADCCON |= (1 << 0);
        }

    } else {
        /* 松开 */

        /* 重置平均 */
        adc_cnt = 0;
        adc_x_sum = 0;
        adc_y_sum = 0;

        /* 不使能定时器读取ADC的值 */
        press_flag_disable();
        /* 进入等待触摸按下模式 */
        touch_screen_wait_down_mode();

        /* 松开,设置触摸屏等待下一次按下状态,才读取触摸屏的数据 */
        get_pen_x_y(0, 0, 0);
    }

    /* 触摸屏测试时进入等待触摸松开模式 */
    /* 启动ADC时进入该模式会影响测量的值 */
    // touch_screen_wait_up_mode();
    
    return;

}

/**
 * @brief 触摸屏的中断控制器
 * 
 * @param irq_no 
 */
void touch_screen_irq_handle(unsigned int irq_no)
{
    /* 判断SUBSRCPND bit10 bit9 */
    
    if ((SUBSRCPND >> INT_TC_BIT) & 1) {
        /* 处理触摸屏中断 */
        tc_irq_handle();
    }
    
    if ((SUBSRCPND >> INT_ADC_S_BIT) & 1) {
        /* 处理ADC中断 */
        adc_x_y_irq_handle();

    }

     /* 清除中断,向清除的位置写入1 */
    SUBSRCPND = (1 << INT_ADC_S_BIT) | (1 << INT_TC_BIT);

    return;
}

void touch_screen_control_init(void)
{
    /* INTSUBMSK 打开INT_ADC_S INT_TC */
    INTSUBMSK &= ~((1 << INT_ADC_S_BIT) | (1 << INT_TC_BIT));

    /* 注册中断函数 */
    register_irq_func(31, touch_screen_irq_handle);
    return;
}

void touch_screen_adc_init(void)
{
    /**
     * ECFLG   bit15-1 转换flag成功
     * PRSCEN  bit14-1 预分频
     * PRSCVL  bit13:bit6-49 预分频值
     * SEL_MUX bit5:bit3- 选择输入ADC引脚
     * STDBM   bit2:1     待机模式
     * READ_ START bit1   可以读取值
     * ENABLE_START bit0   写入1转换使能,需要清除0
     */

    ADCCON = (1 << 14) | (49 << 6) | (0 << 3);

    /** 按下触摸屏, 延时一会再发出TC中断
     *  延时时间 = ADCDLY * 晶振周期 = ADCDLY * 1 / 12000000 = 5ms
    */
    ADCDLY = 60000;

    return;
}

void touch_screen_wait_down_mode(void)
{
    /* 设置触摸屏的开关, S1, S2, S3, S4, S5,进入不同的触摸屏的模式 */
    ADCTSC = DOWN_STAT_BIT | PULL_UP_ENABLE | YM_ENABLE | YP_DISABLE | XM_DISABLE | XP_DISABLE | XY_PST_WAIT;
    
}

void touch_screen_wait_up_mode(void)
{
    /* 设置触摸屏的开关, S1, S2, S3, S4, S5,进入不同的触摸屏的模式 */
    ADCTSC = UP_STAT_BIT | PULL_UP_ENABLE | YM_ENABLE | YP_DISABLE | XM_DISABLE | XP_DISABLE | XY_PST_WAIT;
    
}

void touch_screen_init(void)
{
    /* 初始化中断控制器 */
    touch_screen_control_init();

    /* 初始化触摸屏的ADC相关寄存器*/
    touch_screen_adc_init();

    /* 注册长按定时器函数 */
    register_timer("touch_screen", touch_screen_timer_irq_handle);

    /* 设置触摸屏的开关, S1, S2, S3, S4, S5,进入不同的触摸屏的模式 */
    /* 触摸屏进入"等待中断模式" */
    touch_screen_wait_down_mode();

    return;
}

void auto_mode_get_x_y_pos(void)
{
    /* 设置自动模式获取x和y轴的数据 */
    ADCTSC = AUTO_PST_AUTO | XY_PST_NO_OPR;

}



