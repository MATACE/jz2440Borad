#ifndef GPIO_SPI_H
#define GPIO_SPI_H

#include "s3c2440_soc.h"
#include "my_printf.h"

typedef enum {
    CHIP_E = 0,
    CHIP_D = 1,
} CHIP;

typedef enum {
    CMD_M = 0,
    DATA_M = 1,
} OLED_MODE;

typedef enum {
    P_SET = 1,
    P_UNSET = 0,
} PIN_SET;

int gpio_spi_init(void);
void spi_set_sclk(PIN_SET val);
void spi_set_do(char val);
void spi_send_byte(unsigned char val);
int spi_recv_byte(void);
int spi_get_di(void);
void spi_init();

#endif // GPIO_SPI_H

