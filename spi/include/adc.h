#ifndef ADC_H
#define ADC_H

#include "s3c2440_soc.h"

void adc_init(void);
unsigned int get_ain0_adc_val(void); 

#endif // ADC_H


