#ifndef SPI_OLED_H
#define SPI_OLED_H

#include "gpio_spi.h"
#include "oled_font.h"
#include "my_printf.h"
#include "spi_controler_reg.h"

extern unsigned int SPI_MODE_INIT_FLAG;

void oled_set_cs(CHIP val);
void oled_set_dc(OLED_MODE val);
int oled_write_data(unsigned char data);
int oled_write_cmd(unsigned char data);
int oled_init(void);
void oled_full(void);
void oled_clear(void);
void oled_set_page_addr(void);
void oled_page_pos(unsigned int page, unsigned col);
void oled_putchar(unsigned int page, unsigned int col, char ch);
void oled_print(unsigned int page, unsigned int col, char *buf);


#endif // SPI_OLED_H


