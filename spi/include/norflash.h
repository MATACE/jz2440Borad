#ifndef NORFLASH_H
#define NORFLASH_H

#include "s3c2440_soc.h"

#include "my_printf.h"
#include "string_utils.h"
#include "uart.h"

/* norflash和芯片连接的基地址 */
#define NOR_FLASH_BASE_ADDR 0

int norflash_clock_set(int val);
void norflash_op_menu(void);

int scan_nor_flash(void);
int erase_nor_flash(void);
int write_nor_flash(void);
int read_nor_flash(void);

void nor_flash_write(unsigned int base, unsigned int offset, unsigned int val);
unsigned int nor_flash_read(unsigned int base, unsigned int offset);

void wait_complete(volatile unsigned int addr);

#endif //NORFLASH_H
