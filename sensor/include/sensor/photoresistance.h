#ifndef PHOTORESISTANCE_H
#define PHOTORESISTANCE_H

#include "adc.h"
#include "my_printf.h"
#include "string_utils.h"

void photo_gpio_init();
void get_photoresistance_val(void);

#endif // PHOTORESISTANCE_H
