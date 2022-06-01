#include "spi_gpio_menu.h"
#include "spi_controler_reg.h"
#include "gpio_spi.h"
#include "spi_oled.h"
#include "spi_flash.h"

void spi_gpio_menu(void)
{
    unsigned char ch = ' ';

    if (SPI_MODE_INIT_FLAG == 0) {
        /* 设置GPIO的SPI管脚初始化 */
        spi_init();
    } else {
        /* 设置GPIO的SPI管脚初始化 */
        spi_control_init();
    }

    /* SPI FLASH初始化 */
    spi_flash_init();

    /* SPI测试菜单 */
    
    while (1) {
        /* 输出菜单 */
        printf("[o] Spi Oled Test\n\r");
        printf("[i] Spi Read Flash ID\n\r");
        printf("[r] Spi Read Flash\n\r");
        printf("[w] Spi Write Flash\n\r");
        printf("[e] SPi Flash Erase Bank Addr\n\r");
        printf("[q] quit\n\r");
        printf("Enter selection: ");

        /* 获取字符 */
        ch = getchar();
        printf("%c\n\r", ch);

        switch (ch) {
            case 'o':
                oled_text_show();
            break;
            case 'i':
                spi_flash_read_id();
            break;
            case 'r':
                spi_flash_read();
            break;
            case 'w':
                spi_flash_write();
            break;
            case 'e':
                spi_flash_erase_bank_addr();
            break;
            case 'q':
                return;
            break;
            default:
                printf("Enter secletion err\n\r");
            break;
        }
    }
    return;
}

void spi_flash_write(void)
{
    unsigned int w_addr = 0;
    unsigned char str[128] = {};

    /* 获取输入的地址 */
    printf("Enter the write addr: ");
    w_addr = get_uint();

    /* 判断地址 */
    if (w_addr > 4096) {
        printf("err addr\r\n");
        return;
    }

    printf("Enter the write data:\n\r");
    /* 获取输入的字串 */
    gets(str);

    /* 把值写入spi flash */
    spi_flash_page_program(w_addr, str, strlen(str)+1);
}

void spi_flash_read(void)
{
    /* 清除操作 */
    // spi_flash_erase(4096);

    // /* 获取flash的reg1和reg2 */
    // char buf[2] = {};
    // buf[0] = get_status_reg1();
    // buf[1] = get_status_reg2();
    // printf("reg1 = %d, reg2 = %d\r\n", buf[0], buf[1]);
    
    // /* 往某个地址写入值 */
    // spi_flash_page_program(4096, "Hello", 6);
    /* 获取写入地址的值 */
    
    unsigned int addr = 0;
    unsigned int len = 0;
    unsigned char ch = ' ';
    unsigned char str[16] = {};
    unsigned char data[128] = {};
    int cnt = 0;

    /* 获取地址 */
    printf("Enter the read addr: ");
    addr = get_uint();

    if (addr > 4096) {
        printf("err addr\r\n");
        return;
    }

    /* 获取地址 */
    printf("Enter the read len: ");
    len = get_uint();

    /* 把值从spi flash读出 */
    spi_flash_read_data(addr, data, len);
    // printf("SPI DATA = %s\n\r", data);

    printf("Data:\r\n");
    /* 长度固定为64 */
    for (int i = 0; i < 4; i++) {
        /*每行获取16个字节*/
        for (int j = 0; j < 16; j++) {
            ch = data[cnt++];
            str[j] = ch;
            printf("%02x ", ch);
        }

        printf("    ; ");
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
}

void spi_flash_read_id(void)
{
    flash_read_id();
}

void spi_flash_erase_bank_addr()
{
    unsigned int addr = 0;

    /* 获取地址 */
    printf("Enter the erase addr: ");
    addr = get_uint();

    /* 擦除地址 */
    spi_flash_erase(addr);

}

void oled_text_show(void)
{
    /* OLED初始化 */
    oled_init();
    /* 在某一页显示数据 */
    oled_print(0, 0, "Hello World Good Morning");

}




