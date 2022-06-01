#ifndef SENSOR_MENU_H
#define SENSOR_MENU_H

#include "my_printf.h"
#include "string_utils.h"
#include "adc.h"
#include "photoresistance.h"

void sensor_menu(void);
void photoresistance_test(void);
void dht11_test(void);

#endif // SENSOR_MENU_H
