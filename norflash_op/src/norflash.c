#include "norflash.h"

int norflash_clock_set(int val)
{
    /* 查看主芯片的时序 */
    /* 查看norflash芯片的datasheet */
    /* 设置Tacc需要的时许 >= 70 ns */
    /* HCLK 100M 1/100 * 1000ms *1000us *1000ns = 10ns */
    /* Tacc >= 70 Tacc = 8clock, 10clock, 14clock */

    if (val != 5 && val != 6 && val != 7) {
        BANKCON0 &= ~(7 << 8);
        BANKCON0 |= (7 << 8);
        return -1;
    }

    if (val == 5) {
        BANKCON0 &= ~(7 << 8);
        BANKCON0 |= (5 << 8);
    } else if (val == 6) {
        BANKCON0 &= ~(7 << 8);
        BANKCON0 |= (6 << 8);
    } else if (val == 7) {
        BANKCON0 &= ~(7 << 8);
        BANKCON0 |= (7 << 8);
    }

    return 0;
}

void norflash_op_menu(void)
{
    /**
     * 识别nor flash
     * 擦除nor flash的某个区域
     * 编写某个地址
     * 读写某个地址
     */

    unsigned char ch = ' ';
    
    while (1) {
        /* 输出菜单 */
        printf("[s] Scan nor flash\n\r");
        printf("[e] Erase nor flash\n\r");
        printf("[w] Write nor flash\n\r");
        printf("[r] Read nor flash\n\r");
        printf("[q] quit\n\r");
        printf("Enter selection: ");

        /* 获取字符 */
        ch = getchar();
        printf("%c\n\r", ch);

        switch (ch) {
            case 's':
                scan_nor_flash();
            break;
            case 'e':
                erase_nor_flash();
            break;
            case 'w':
                write_nor_flash();
            break;
            case 'r':
                read_nor_flash();
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

int scan_nor_flash(void)
{
    /* 芯片信息的数组 */
    char inf_qry_arry[4] = {};
    unsigned int size_val = 0;
    int regions_num = 0;
    unsigned int dev_id = 0;
    unsigned int mani_id = 0;
    unsigned int regions_blocks = 0;
    unsigned int regions_block_size = 0;
    unsigned int regions_block_base_addr = 0x2d;
    unsigned int block_addr = 0;
    unsigned int cnt = 0;

    /* 打印厂家id\设备id */
    /* 解锁信号 */
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0xaa);
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x2aa, 0x55);
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0x90);
    /* 读取厂家ID */
    mani_id = nor_flash_read(NOR_FLASH_BASE_ADDR, 0x0);
    dev_id = nor_flash_read(NOR_FLASH_BASE_ADDR, 0x1);

    printf("Manifacture ID = %x, Device ID = %x\n\r", mani_id, dev_id);
    
    /* 重置芯片, reset */
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x0, 0xf0);

    /* 进入CFI模式,才能读取芯片的信息 */
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x55, 0x98);
    
    inf_qry_arry[0] = nor_flash_read(NOR_FLASH_BASE_ADDR, 0x10);
    inf_qry_arry[1] = nor_flash_read(NOR_FLASH_BASE_ADDR, 0x11);
    inf_qry_arry[2] = nor_flash_read(NOR_FLASH_BASE_ADDR, 0x12);
    inf_qry_arry[3] = '\0';

    printf("CFI INF: %s\n\r", inf_qry_arry);

    /* 打印容量 */
    size_val = nor_flash_read(NOR_FLASH_BASE_ADDR, 0x27);
    printf("NOR FLASH SIZE: %dM\n\r", (1 << size_val)/(1024*1024));

    /* 打印各个扇区的起始地址 */
    /**
     * erase block regions: 含有1个或者多个block, 一样的大小
     * 1个nor flash含有1个或多个region
     * 1个region含有1个或多个block(扇区)
     * 
     * Erase block region 1 information：
     * 低两字节+1 : 表示该region有多少个block
     * 高两字节*256 : 表示block的大小 
     */
    
    /* 获取regions的个数 */
    regions_num = nor_flash_read(NOR_FLASH_BASE_ADDR, 0x2c);

    /* 获取每个block的相关信息 */
    for (int i = 0; i < regions_num; i++) {
        /** 获取高两字节数据获取扇区的容量大小
         * [30, 2F] = sector size in multiples of 256-bytes
         * 十六进制4bit位
         */
        regions_blocks = nor_flash_read(NOR_FLASH_BASE_ADDR, regions_block_base_addr)\
                        + (nor_flash_read(NOR_FLASH_BASE_ADDR, regions_block_base_addr+1) << 8)+1;
        
        /** 获取低两字节+1表示该region有多少个block
         * [2E,2D] = # of same-size sectors in region 1-1
         * 
         */
        regions_block_size = 256 * (nor_flash_read(NOR_FLASH_BASE_ADDR, regions_block_base_addr+2) +\
                                    (nor_flash_read(NOR_FLASH_BASE_ADDR, regions_block_base_addr+3) << 8));

        /* 切换到小一个regions块,地址加4 */
        regions_block_base_addr += 4;
        
        // printf("block %d\n\r", regions_blocks);
        // print_hex(regions_block_size);
        // printf("\n\r");

        for (int j = 0; j < regions_blocks; j++) {
            print_hex(block_addr);
            putchar(' ');
            block_addr += regions_block_size;
            cnt++;
            if (cnt == 5) {
                cnt = 0;
                printf("\n\r");
            }
        }
    }

    /* 退出CFI模式, reset */
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x0, 0xf0);

    return 0;
}

int erase_nor_flash(void)
{
    unsigned int addr = 0;

    /* 获取输入的地址 */
    printf("Enter the erase addr: ");
    addr = get_uint();

    /* 解锁信号 */
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0xaa);
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x2aa, 0x55);
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0x80);
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0xaa);
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x2aa, 0x55);

    /* 擦除地址 */
    /** CPU的地址线A1接到Norflash的A0上
     *  nor_flash_write中已经将数据左移一位
     *  在擦除操作中需要将传递进来的数据右移动一位保证擦除的地址不变
     */
    nor_flash_write(NOR_FLASH_BASE_ADDR, addr >> 1, 0x30);

    /* 等待Norflash操作完成 */
    wait_complete(addr);

    /* 退出命令模式, reset */
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x0, 0xf0);

    return 0;
}

int write_nor_flash(void)
{
    unsigned int w_addr = 0;
    unsigned char str[128];
    unsigned int i = 0;

    /* 获取输入的地址 */
    printf("Enter the write addr: ");
    w_addr = get_uint();

    printf("Enter the write data:\n\r");
    /* 获取输入的字串 */
    gets(str);

    /* 写入数据 */ 
    /* 合并两个字节为1个数据 */
    while (str[i] != '\0' && str[i+1] != '\0') {
        /** CPU的地址线A1接到Norflash的A0上
         *  nor_flash_write中已经将数据左移一位
         *  在擦除操作中需要将传递进来的数据右移动一位保证擦除的地址不变
         */
        /* 解锁写入Program信号 */
        nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0xaa);
        nor_flash_write(NOR_FLASH_BASE_ADDR, 0x2aa, 0x55);
        nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0xa0);
        /* 要写入一个word,两个字节 */
        nor_flash_write(NOR_FLASH_BASE_ADDR, w_addr >> 1, (str[i] + (str[i+1] << 8)));
        /* 等待数据写入完成 */
        wait_complete(w_addr);
        /* 地址增加 */
        w_addr += 2;
        i += 2;
    }

    /* 结尾字节操作写入 */
    if (str[i] == '\0') {

    } else {
        /* 解锁写入Program信号 */
        nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0xaa);
        nor_flash_write(NOR_FLASH_BASE_ADDR, 0x2aa, 0x55);
        nor_flash_write(NOR_FLASH_BASE_ADDR, 0x555, 0xa0);
        nor_flash_write(NOR_FLASH_BASE_ADDR, w_addr >> 1, str[i] + (0x0 << 8));
        wait_complete(w_addr);
    }

    /* 退出命令模式, reset */
    nor_flash_write(NOR_FLASH_BASE_ADDR, 0x0, 0xf0);

    return 0;
}

int read_nor_flash(void)
{
    volatile unsigned char *p_addr;
    unsigned int addr = 0;
    unsigned char ch = ' ';
    unsigned char str[17];

    /* 获取地址 */
    printf("Enter the read addr: ");
    addr = get_uint();

    /* 此时读的是数据的地址, D0对应的是Norflash上的D0 */
    p_addr = (volatile unsigned char *)addr;

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

/**
 * @brief 往Norflash地址写入某个值
 * 
 * @param base      norflash在芯片上的基地址
 * @param offset    操作norflash的地址值
 * @param val       写入的值
 */
void nor_flash_write(unsigned int base, unsigned int offset, unsigned int val)
{
    /** 芯片的A1地址和norflash的A0地址相联,
     * CPU发出地址时地址要左移一位才访问到norflash地址 
     */

    volatile unsigned short *p_addr = (volatile unsigned short *)(base + (offset << 1));
    *p_addr = val;
}

unsigned int nor_flash_read(unsigned int base, unsigned int offset)
{
    /** 芯片的A1地址和norflash的A0地址相联,
     * CPU发出地址时地址要左移一位才访问到norflash地址 
     */

    volatile short int *p_addr = (volatile short int *)(base + (offset << 1));
    return *p_addr;
}

/**
 * @brief 等待norflash操作完成
 * 
 * @param addr 操作的地址
 */
void wait_complete(volatile unsigned int addr)
{   
    /**
     * 读取擦除和烧写的地址的值,bit6变化表示烧写未完成
     * Toggle Bit I on Q6 indicates whether an 
     * Automatic Program or Erase algorithm is in 
     * progress or complete
     */
    unsigned int pre_val = 0;
    unsigned int val = 0;

    /** CPU的地址线A1接到Norflash的A0上
     *  nor_flash_write中已经将数据左移一位
     *  在擦除操作中需要将传递进来的数据右移动一位保证擦除的地址不变
     */
    pre_val = nor_flash_read(NOR_FLASH_BASE_ADDR, addr >> 1);
    val = nor_flash_read(NOR_FLASH_BASE_ADDR, addr >> 1);
    /* 比较当前的值和前一次值的bit6是否相同 */
    while (((pre_val >> 6) & 1) != ((val >> 6) & 1)) {
        /* 保存当前的值  */
        pre_val = val;
        /* 获取当前的值 */
        val = nor_flash_read(NOR_FLASH_BASE_ADDR, addr >> 1);
    }

    return;
}

