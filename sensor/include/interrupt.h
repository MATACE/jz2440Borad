#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "s3c2440_soc.h"

#include "uart.h"
#include "common.h"
#include "my_printf.h"
#include "string_utils.h"

typedef void (*irq_handle_func)(const unsigned int irq_no);
/* irq处理函数 */
extern irq_handle_func irq_func_array[64];

void register_irq_func(const unsigned int irq_no, irq_handle_func func);
void interrupt_control_init(void);
void interrupt_key_init(void);
void irq_handle(void);
void key_irq_handle(unsigned int irq_no);


#endif // INTERRUPT_H
