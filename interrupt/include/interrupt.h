#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "s3c2440_soc.h"

#include "uart.h"

void interrupt_control_init(void);
void interrupt_key_init(void);
void irq_handle(void);
void key_irq_handle(unsigned int irq_no);

#endif // INTERRUPT_H