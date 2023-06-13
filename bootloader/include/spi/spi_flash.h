#ifndef SPI_FLASH_H
#define SPI_FLASH_H

#include "gpio_spi.h"
#include "spi_controler_reg.h"

typedef enum {
    DIS = 0,
    ENA = 1
} SPI_OP_STA;

extern unsigned int SPI_MODE_INIT_FLAG;

void flash_set_cs(CHIP val);
void flash_write_data(unsigned char data);
int flash_recv_data(void);
void flash_read_id(void);
void flash_write_enable(SPI_OP_STA val);
int get_status_reg1(void);
int get_status_reg2(void);
void flash_protect_reg_clear(void);
void spi_flash_init();
void write_status_reg1_reg2(unsigned char reg1, unsigned char reg2);
void flash_protect_data_clear(void);
void spi_flash_wait_busy(void);
void spi_flash_read_data(unsigned int addr, unsigned char *buf, unsigned int len);
void spi_flash_erase(unsigned int addr);
void spi_flash_page_program(unsigned int addr, unsigned char *buf, unsigned int len);
void send_flash_addr(unsigned int addr);

#endif // SPI_FLASH_H

