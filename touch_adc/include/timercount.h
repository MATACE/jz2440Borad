#ifndef TIMERCOUNT_H
#define TIMERCOUNT_H

#include "s3c2440_soc.h"

#include "uart.h"
#include "interrupt.h"
#include "my_printf.h"
#include "string_utils.h"

#define TIME_NUM 32

typedef void(*timer_func)(void);

typedef struct timer_desc {
    char *name;
    timer_func fp;
} TIMER_DESC, P_TIMER_DESC;

void inter_contr_timer0_init(void);
void timer_init(void);

void timer_irq(const unsigned int irq_no);
int register_timer(char *name, timer_func fp);
int unregister_timer(char *name);

#endif // TIMERCOUNT_H
