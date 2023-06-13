#ifndef SPI_CONTROLER_H
#define SPI_CONTROLER_H

#include "s3c2440_soc.h"
#include "my_printf.h"
#include "string_utils.h"

void spi_control_init(void);
int spi_control_gpio_init(void);
int spi_control_recv(void);
void spi_control_send(unsigned char val);


#endif // SPI_CONTROLER_H

