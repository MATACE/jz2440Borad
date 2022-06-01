#include "nandflash.h"

void nand_init(void)
{
    /* Nandflash芯片时序图 */
    /* CLE/ALE/nWE的最小值都是12ns,可以同时发出 TACLS=0 */
    /* twp要>=12ns,TWRPH0=1 10(ns)*(TWRPH0+1)>=12ns */
    /* CLE/ALE在nWE后等待释放的时间>=5ns TWRPH1=0 10(ns)*(TWRPH1+1)>=5ns */
    // NFCONF &= ~((3 << 12) | (7 << 8) | (7 << 4));
    // NFCONF |= (0 << 12) | (1 << 8) | (0 << 4);
    /* 寄存器只能直接通过赋值初始化 */
    NFCONF = (0 << 12) | (1 << 8) | (0 << 4);

    /* Nandflash初始化 */
    // NFCONT &= ~((1 << 0) | (1 << 1)| (1 << 4));
    // NFCONT |= (1 << 0) | (1 << 1) | (1 << 4);
    /* 寄存器只能直接通过赋值初始化 */
    NFCONT = (1 << 0) | (1 << 1) | (1 << 4);
}

int read_nand(unsigned int addr, unsigned char *buf, unsigned int len)
{
    /* 获取Nandflash上的数据位置*/
    /* 获取页的数据的位置 */
    unsigned int page_row = addr / 2048;
    /* 获取数据所在页的列 */
    unsigned int page_col = addr & (2048 - 1);
    unsigned int i = 0;

    /* 选中片选 */
    chip_select_enable();

    while (i < len) {
        /* 获取一页中的数据 */
        /* 发送开始命令 */
        chip_cmd(0x00);

        /* 发送行地址 */
        /* 发送地址的A0到A7 */
        chip_addr(page_col & 0xff);
        /* 发送地址的A8到A11 */
        chip_addr((page_col >> 8) & 0xff);
        
        /* 发送列地址 */
        /* 发送地址A12到A19 */
        chip_addr(page_row & 0xff);
        /* 发送地址A20到A27 */
        chip_addr((page_row >> 8) & 0xff);
        /* 发送地址A28 */
        chip_addr((page_row >> 16) & 0xff);

        /* 发送页结束命令 */
        chip_cmd(0x30);

        /* 等待返回数据 */
        nand_wait_complete();

        /* 每一页的数据不超过2048,读取的总数据不超过长度 */
        while ((page_col < 2048) && (i < len)) {
            buf[i++] = nand_data();
            page_col++;
        }

        /* 当len读取数据和i相同时读完数据 */
        if (i == len) {
            break;
        }

        page_col = 0;
        page_row++;
        
    }
    /* 释放片选  */
    chip_select_disable();
    
    return 0;
}

void chip_select_enable(void)
{
    /* 片选选中 */
    NFCONT &= ~(1 << 1);
}

void chip_select_disable(void)
{
    /* 取消片选 */
    NFCONT |= (1 << 1);
}

void chip_cmd(volatile unsigned char cmd)
{
    NFCMMD = cmd;
    /* 延时等待 */
    for (int i = 10; i >= 0; i--);
}

void chip_addr(volatile unsigned char addr)
{
    NFADDR = addr;
    /* 延时等待 */
    for (int i = 10; i >= 0; i--);
}

void write_nand_data(volatile unsigned char data)
{
    NFDATA = data;
}

unsigned int nand_data(void)
{
    return NFDATA;
}

void nand_wait_complete(void)
{
    /* 获取NFSTAT中的输入引脚的状态 */
    while (!(NFSTAT & 1));
}

int scan_nand_flash(void)
{
    unsigned char buf[5] = {};

    /* 选中Nandflash片选 */
    chip_select_enable();
    /* 发送命令 */
    chip_cmd(0x90);
    /* 发送地址 */
    chip_addr(0x00);

    /* 获取返回数据 */
    buf[0] = nand_data();
    buf[1] = nand_data();
    buf[2] = nand_data();
    buf[3] = nand_data();
    buf[4] = nand_data();

    printf("Maker Code:0x%x\n\r", buf[0]);
    printf("Device Code:0x%x\n\r", buf[1]);
    printf("Internal Chip Number:%d, Cell Type:%d, Number of Simultaneously Programmed Pages:%d\r\n",\
        1 << (buf[2] & 3), 2*(1 << ((buf[2] >> 2) & 3)), 1 << ((buf[2] >> 4) & 3));
    printf("Page Size:%dKB, Block Size:%dKB\n\r",\
        1 << (buf[3] & 3), 64*(1 << ((buf[3] >> 4) & 3)));

    printf("Plane Number:%dMB, Plane Size:%dMB\n\r",\
        1 << ((buf[4] >> 2) & 3), 64*(1 << ((buf[4] >> 4) & 7)));

    /* 关闭Nandflash片选 */
    chip_select_disable();

    return 0;
}

int read_nand_flash(void)
{
    volatile unsigned char *p_addr;
    unsigned int addr = 0;
    unsigned char ch = ' ';
    unsigned char str[17];
    unsigned char buf[64];

    /* 获取地址 */
    printf("Enter the read addr: ");
    addr = get_uint();

    /* 此时读取Nandflash上的数据 */
    read_nand(addr, buf, 64);
    p_addr = (volatile unsigned char *)buf;

    printf("Data:\n\r");
    /* 长度固定为64 */
    for (int i = 0; i < 4; i++) {
        /*每行获取16个字节*/
        for (int j = 0; j < 16; j++) {
            ch = *p_addr++;
            str[j] = ch;
            printf("%02x ", ch);
        }

        printf("    ; ");
        /* 打印字符 */
        str[16] = '\0';
        for (int j = 0; j < 16; j++) {
            /* 打印不可视字符用.替换*/
            if (str[j] < 0x20 || str[j] > 0x7e) {
                putchar('.');
            } else {
                putchar(str[j]);
            }
        }
        printf("\n\r");
    }
    return 0;
}

int write_nand(unsigned int addr, unsigned char *buf, unsigned int len)
{
    /* 获取Nandflash上的数据位置*/
    /* 获取页的数据的位置 */
    unsigned int page_row = addr / 2048;
    /* 获取数据所在页的列 */
    unsigned int page_col = addr & (2048 - 1);
    unsigned int i = 0;

    /* 选中片选 */
    chip_select_enable();

    while (1) {
        /* 获取一页中的数据 */
        /* 发送开始命令 */
        chip_cmd(0x80);

        /* 发送行地址 */
        /* 发送地址的A0到A7 */
        chip_addr(page_col & 0xff);
        /* 发送地址的A8到A11 */
        chip_addr((page_col >> 8) & 0xff);
        
        /* 发送列地址 */
        /* 发送地址A12到A19 */
        chip_addr(page_row & 0xff);
        /* 发送地址A20到A27 */
        chip_addr((page_row >> 8) & 0xff);
        /* 发送地址A28 */
        chip_addr((page_row >> 16) & 0xff);

        /* 每一页的数据不超过2048,读取的总数据不超过长度 */
        while ((page_col < 2048) && (i < len)) {
            /* 写入数据 */
            write_nand_data(buf[i++]);
        }

        /* 发送写结束命令 */
        chip_cmd(0x10);
        /* 等待返回数据 */
        nand_wait_complete();

        /* 当len读取数据和i相同时读完数据 */
        if (i == len) {
            break;
        } else {
            page_col = 0;
            page_row++;
        }   
    }

    /* 释放片选  */
    chip_select_disable();

    return 0;
}

int write_nand_flash(void)
{
    unsigned int w_addr = 0;
    unsigned char str[128] = {};

    /* 获取输入的地址 */
    printf("Enter the write addr: ");
    w_addr = get_uint();

    printf("Enter the write data:\n\r");
    /* 获取输入的字串 */
    gets(str);

    write_nand(w_addr, str, strlen(str) + 1);

    return 0;
}

int erase_nand(unsigned int addr, unsigned int len)
{
    int page_row = addr / 2048;

    if (addr & (0x1FFFF)) {
        printf("nand_erase err, addr is not block align\n\r");
        return -1;
    }

    if (len & (0x1FFFF)) {
        printf("nand_erase err, len is not block align\n\r");
        return -1;
    }

    chip_select_enable();

    while (1) {
        page_row = addr / 2048;

        /* 发送块擦除初始化命令 */
        chip_cmd(0x60);

        /* 发送列地址 */
        /* 发送地址A12到A19 */
        chip_addr(page_row & 0xff);
        /* 发送地址A20到A27 */
        chip_addr((page_row >> 8) & 0xff);
        /* 发送地址A28 */
        chip_addr((page_row >> 16) & 0xff);

        /* 发送擦除命令 */
        chip_cmd(0xD0);

        nand_wait_complete();

        len -= (128*1024);
        if (len == 0) {
            break;
        }    
        addr += (128*1024);
    }

    chip_select_disable();
    printf("erase ok\n\r");
    return 0;
}

int erase_nand_flash(void)
{
    unsigned int addr = 0;

    /* 获取输入的地址 */
    printf("Enter the erase addr: ");
    addr = get_uint();

    /* 一个块是128kb */
    erase_nand(addr, 128*1024);

    return 0;
}

void nandflash_op_menu(void)
{
    /**
     * 识别nand flash
     * 擦除nand flash的某个区域
     * 编写某个地址
     * 读写某个地址
     */

    unsigned char ch = ' ';
    
    while (1) {
        /* 输出菜单 */
        printf("[s] Scan Nand flash\n\r");
        printf("[e] Erase Nand flash\n\r");
        printf("[w] Write Nand flash\n\r");
        printf("[r] Read Nand flash\n\r");
        printf("[q] quit\n\r");
        printf("Enter selection: ");

        /* 获取字符 */
        ch = getchar();
        printf("%c\n\r", ch);

        switch (ch) {
            case 's':
                scan_nand_flash();
            break;
            case 'e':
                erase_nand_flash();
            break;
            case 'w':
                write_nand_flash();
            break;
            case 'r':
                read_nand_flash();
            break;
            case 'q':
                return;
            break;
            default:
                printf("Enter secletion err\n\r");
            break;
        }
    }
}

