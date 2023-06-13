#ifndef NANDFLASH_H
#define NANDFLASH_H

#include "s3c2440_soc.h"
#include "my_printf.h"
#include "string_utils.h"
#include "common.h"

void nand_init(void);

int scan_nand_flash(void);
int read_nand_flash(void);
int read_nand(unsigned int addr, unsigned char *buf, unsigned int len);
int write_nand_flash(void);
int write_nand(unsigned int addr, unsigned char *buf, unsigned int len);
int erase_nand(unsigned int addr, unsigned int len);
int erase_nand_flash(void);
void nand_wait_complete(void);

void chip_select_enable(void);
void chip_select_disable(void);
void chip_cmd(volatile unsigned char cmd);
void chip_addr(volatile unsigned char addr);
unsigned int nand_data(void);
void write_nand_data(volatile unsigned char data);
void nandflash_op_menu(void);

#endif // NANDFLASH_H

