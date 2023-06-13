#ifndef DHT11_H
#define DHT11_H

#include "my_printf.h"
#include "string_utils.h"
#include "s3c2440_soc.h"
#include "timercount.h"

typedef enum {
    DTH_O = 0,
    DTH_I = 1,
} DTH_MODE;

typedef enum {
    DTH_L = 0,
    DTH_H = 1,
} DTH_PIN;

void set_dth11_gpio_mode(DTH_MODE mode);
void set_dth11_gpio_pin(DTH_PIN pin);
DTH_PIN get_dht11_gpio_pin(void);
void dth11_init(void);
int dht11_wait_ack_pin(DTH_PIN val, int time_out);
void dht11_start(void);
int dht11_recv_byte(void);
int dht11_ready_read(int *hum, int *temp);


#endif //DHT11_H

