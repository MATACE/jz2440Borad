#ifndef LED_H
#define LED_H

#include "s3c2440_soc.h"
#include "common.h"
#include "timercount.h"

void led_init(void);
void key_led_init(void);
void key_action(void);
void led_irq_handle(void);

#endif // LED_H
