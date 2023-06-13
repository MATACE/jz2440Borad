#ifndef TIMERCOUNT_H
#define TIMERCOUNT_H

#include "s3c2440_soc.h"

#include "uart.h"
#include "interrupt.h"

void inter_contr_timer0_init(void);
void timer_init(void);
void timer0_irq_handle(const unsigned int irq_no);

#endif // TIMERCOUNT_H
