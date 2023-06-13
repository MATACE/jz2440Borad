#include "spi_flash.h"

void flash_set_cs(CHIP val)
{
    if (val == CHIP_D) {
        GPGDAT |= (1 << 2);
    } else if (val == CHIP_E) {
        GPGDAT &= ~(1 << 2);
    }
}

void flash_write_data(unsigned char data)
{
    /* 设置flash的CS引脚选中 */
    flash_set_cs(CHIP_E);

    /* 从SPI中发送数据 */
    if (SPI_MODE_INIT_FLAG == 0) {
        spi_send_byte(data);
    } else {
        spi_control_send(data);
    }

    /* 首先应该取消片选 */
    /* 设置flash的CS引脚选中 */
    flash_set_cs(CHIP_D);
}

int flash_recv_data(void)
{
    unsigned char data = 0;
    /* 设置flash的CS引脚选中 */
    flash_set_cs(CHIP_E);

    if (SPI_MODE_INIT_FLAG == 0) {
        /* 从SPI中发送数据 */
        data = spi_recv_byte();
    } else {
        /* 从SPI中发送数据 */
        data = spi_control_recv();
    }

    /* 设置flash的CS引脚选中 */
    flash_set_cs(CHIP_D);

    return data;
}

void flash_read_id(void)
{
    unsigned char buf[2] = {};
    /* 设置flash的CS引脚选中 */
    flash_set_cs(CHIP_E);

    if (SPI_MODE_INIT_FLAG == 0) {
        /* 发送flash的命令,读取ID */
        spi_send_byte(0x90);
        
        /* 根据时序图的要求,发送3个0x0数据 */
        spi_send_byte(0x0);
        spi_send_byte(0x0);
        spi_send_byte(0x0);

        /* 读取到flash的一个字节的数据 */
        buf[0] = spi_recv_byte();
        buf[1] = spi_recv_byte();
    } else {
        /* 发送flash的命令,读取ID */
        spi_control_send(0x90);
        
        /* 根据时序图的要求,发送3个0x0数据 */
        spi_control_send(0x0);
        spi_control_send(0x0);
        spi_control_send(0x0);

        /* 读取到flash的一个字节的数据 */
        buf[0] = spi_control_recv();
        buf[1] = spi_control_recv();
    }

    /* 不设置flash的CS引脚选中 */
    flash_set_cs(CHIP_D);

    printf("MID = 0x%x, DID = 0x%x\r\n", buf[0], buf[1]);

}

void flash_write_enable(SPI_OP_STA val)
{
    /* flash的操作需要写使能 */

    /* 选中flash的片选 */
    flash_set_cs(CHIP_E);

    if (val == ENA) {
        /* Write Enable (06h) */
        if (SPI_MODE_INIT_FLAG == 0) {
            spi_send_byte(0x06);
        } else {
            spi_control_send(0x06);
        }
    } else if (val == DIS) {
        /* Write Disable (04h) */
        if (SPI_MODE_INIT_FLAG == 0) {
            spi_send_byte(0x04);
        } else {
            spi_control_send(0x04);
        }
    }

    /* 释放flash的片选 */
    flash_set_cs(CHIP_D);
}

int get_status_reg1(void)
{
    /* 获取flash的保护状态寄存器一 */
    unsigned char val1 = 0;
    // unsigned char val2 = 0;
    
    /* 选中flash的片选 */
    flash_set_cs(CHIP_E);

    /* 根据时序图 */
    /* 发送读取reg1的第一个的byte数据 */
    if (SPI_MODE_INIT_FLAG == 0) {
        spi_send_byte(0x05);
        /* 接受reg1的数据 */
        val1 = spi_recv_byte();
    } else {
        spi_control_send(0x05);
        /* 接受reg1的数据 */
        val1 = spi_control_recv();
    }

    /* 取消选中flash的片选 */
    flash_set_cs(CHIP_D);

    return val1;
    
}

int get_status_reg2(void)
{
    /* 获取flash的保护状态寄存器一 */
    unsigned char val1 = 0;
    // unsigned char val2 = 0;
    
    /* 选中flash的片选 */
    flash_set_cs(CHIP_E);

    /* 根据时序图 */
    if (SPI_MODE_INIT_FLAG == 0) {
        /* 发送读取reg1的第一个的byte数据 */
        spi_send_byte(0x35);
        /* 接受reg1的数据 */
        val1 = spi_recv_byte();
    } else {
        /* 发送读取reg1的第一个的byte数据 */
        spi_control_send(0x35);
        /* 接受reg1的数据 */
        val1 = spi_control_recv();
    }

    /* 取消选中flash的片选 */
    flash_set_cs(CHIP_D);
    
    return val1;

}

void write_status_reg1_reg2(unsigned char reg1, unsigned char reg2)
{
    /* 写non-volatile的数据需要先开始写使能 */
    flash_write_enable(ENA);

    /* 写保护寄存器的reg1和reg2 */
    
    /* 选中flash的片选 */
    flash_set_cs(CHIP_E);

    /* 根据时序图 */
    if (SPI_MODE_INIT_FLAG == 0) {
        /* 发送写操作的第一个的byte数据 */
        spi_send_byte(0x01);
        /* 发送reg1的byte数据 */
        spi_send_byte(reg1);

        /* 发送reg2的byte数据 */
        spi_send_byte(reg2);
    } else {
        /* 发送写操作的第一个的byte数据 */
        spi_control_send(0x01);
        /* 发送reg1的byte数据 */
        spi_control_send(reg1);

        /* 发送reg2的byte数据 */
        spi_control_send(reg2);
    }

    /* 取消选中flash的片选 */
    flash_set_cs(CHIP_D);

    /* 等待SPI FLASH完成 */
    spi_flash_wait_busy();

}

void flash_protect_reg_clear(void)
{
    /* 获取写保护位的寄存器reg1和reg2,重置为工厂模式 */
    unsigned char reg1 = 0;
    unsigned char reg2 = 0;

    reg1 = get_status_reg1();
    reg2 = get_status_reg2();

    /* SRP1 = 0, SRP0 = 0 */
    /* SPR0---reg1 bit7 */
    /* SPR1---reg2 bit0 */

    /* 设置SRP1 = 0 */
    reg1 &= ~(1 << 7);
    /* 设置SRP0 = 0 */
    reg2 &= ~(1 << 0);

    /* 写reg1和reg2 */
    write_status_reg1_reg2(reg1, reg2);    
}

void flash_protect_data_clear(void)
{
    /* 获取写保护位的寄存器reg1和reg2, 取消数据存储区的写保护 */
    unsigned char reg1 = 0;
    unsigned char reg2 = 0;

    reg1 = get_status_reg1();
    reg2 = get_status_reg2();

    /* CMP = 0, BP2 = 0, BP1 = 0, BP0 = 0 */
    /* CMP---reg2 bit6 */
    /* BP2---reg1 bit4 */
    /* BP1---reg1 bit3 */
    /* BP0---reg1 bit2 */

    /* 设置BP2 = 0, BP1 = 0, BP0 = 0 */
    reg1 &= ~((1 << 4) | (1 << 3) | (1 << 2));

    /* 设置CMP = 0 */
    reg2 &= ~(1 << 6);

    /* 写reg1和reg2 */
    write_status_reg1_reg2(reg1, reg2);    

}

void send_flash_addr(unsigned int addr)
{
    /* 通过SPI每次发送8bit的数据 */
    if (addr > 0xffffff) {
        printf("Send Spi Flash Addr Failed, Out Of Range\r\n");
        return;
    }
    if (SPI_MODE_INIT_FLAG == 0) {
        spi_send_byte((addr >> 16) & 0xff);
        spi_send_byte((addr >> 8) & 0xff);
        spi_send_byte(addr & 0xff);
    } else {
        spi_control_send((addr >> 16) & 0xff);
        spi_control_send((addr >> 8) & 0xff);
        spi_control_send(addr & 0xff);
    }
}

void spi_flash_page_program(unsigned int addr, unsigned char *buf, unsigned int len)
{
    /* SPI FLASH 写操作 */

    /* 写数据需要先开始写使能 */
    flash_write_enable(ENA);

    /* 选中flash的片选 */
    flash_set_cs(CHIP_E);

    /* 根据时序图Page Program */
    /* 发送1个byte的数据 */
    if (SPI_MODE_INIT_FLAG == 0) {
        spi_send_byte(0x02);
    } else {
        spi_control_send(0x02);
    }

    /* 发送3个byte的地址数据 */
    send_flash_addr(addr);

    /* 发送len个spi数据 */
    for (unsigned int i = 0; i < len; i++) {
        if (SPI_MODE_INIT_FLAG == 0) {
            spi_send_byte(buf[i]);
        } else {
            spi_control_send(buf[i]);
        }
    }

    /* 取消选中flash的片选 */
    flash_set_cs(CHIP_D);

    /* 等待SPI FLASH写入完成 */
    spi_flash_wait_busy();

}

void spi_flash_read_data(unsigned int addr, unsigned char *buf, unsigned int len)
{
    /* SPI FLASH 写操作 */

    /* 选中flash的片选 */
    flash_set_cs(CHIP_E);

    /* 根据时序图Page Program */
    /* 发送1个byte的数据 */
    if (SPI_MODE_INIT_FLAG == 0) {
        spi_send_byte(0x03);
    } else {
        spi_control_send(0x03);
    }

    /* 发送3个byte的地址数据 */
    send_flash_addr(addr);

    /* 发送len个spi数据 */
    for (unsigned int i = 0; i < len; i++) {
        if (SPI_MODE_INIT_FLAG == 0) {
            buf[i] = spi_recv_byte();
        } else {
            buf[i] = spi_control_recv();
        }
    }

    /* 取消选中flash的片选 */
    flash_set_cs(CHIP_D);

}

void spi_flash_wait_busy(void)
{
    /* Wait Spi Flash Busy */
    /* 通过寄存器获取BUSY的状态, 寄存器1的bit0的BUSY时设置成1 */
    while(get_status_reg1() & 0x1);
}

void spi_flash_erase(unsigned int addr)
{
    /* 写数据需要先开始写使能 */
    flash_write_enable(ENA);

    /* 选中flash的片选 */
    flash_set_cs(CHIP_E);

    /* 根据时序图Sector Erase */
    /* 发送1个byte的数据 */
    if (SPI_MODE_INIT_FLAG == 0) {
        spi_send_byte(0x20);
    } else {
        spi_control_send(0x20);
    }

    /* 发送3个byte的地址数据 */
    send_flash_addr(addr);    

    /* 取消选中flash的片选 */
    flash_set_cs(CHIP_D);

    /* 等待SPI FLASH擦除完成 */
    spi_flash_wait_busy();
}

void spi_flash_init(void)
{
    /* 清除SPI FLASH寄存器的写保护位 */
    flash_protect_reg_clear();
    /* 清除SPI FLASH所有数据存储区的写保护位 */
    flash_protect_data_clear();

}




