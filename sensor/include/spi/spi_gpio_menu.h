#ifndef SPI_GPIO_MENU_H
#define SPI_GPIO_MENU_H

#include "my_printf.h"
#include "string_utils.h"

extern unsigned int SPI_MODE_INIT_FLAG;

void spi_gpio_menu(void);
void oled_text_show(void);
void spi_flash_read_id(void);
void spi_flash_read(void);
void spi_flash_write(void);
void spi_flash_erase_bank_addr();

#endif // SPI_GPIO_MENU_H

