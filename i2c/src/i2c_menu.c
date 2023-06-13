#include "uart.h"
#include "my_printf.h"
#include "string_utils.h"
#include "i2c_controler.h"
#include "at2440.h"
#include "i2c_menu.h"
#include "common.h"

void i2c_menu(void)
{
    unsigned char ch = ' ';

    /* 设置I2c控制器 */
    i2c_init();

    /* I2c测试菜单 */
    
    while (1) {
        /* 输出菜单 */
        printf("[w] Write at24cxx\n\r");
        printf("[r] Read at24cxx\n\r");
        printf("[q] quit\n\r");
        printf("Enter selection: ");

        /* 获取字符 */
        ch = getchar();
        printf("%c\n\r", ch);

        switch (ch) {
            case 'w':
                do_at24cxx_write();
            break;
            case 'r':
                do_at24cxx_read();
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

void do_at24cxx_read(void)
{
    unsigned int addr = 0;
    unsigned int len = 0;
    unsigned char ch = ' ';
    unsigned char str[16] = {};
    unsigned char data[100] = {};
    int err = 0;
    int cnt = 0;

    /* 获取地址 */
    printf("Enter the read addr: ");
    addr = get_uint();

    if (addr > 256) {
        printf("err addr\r\n");
        return;
    }

    /* 获取地址 */
    printf("Enter the read len: ");
    len = get_uint();

    /* 把值从at24cxx读出 */
    err = at2440_read(addr, data, len);
    if (err) {
        printf("at2440 read err %d\r\n", err);
    }

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

void do_at24cxx_write(void)
{
    unsigned int w_addr = 0;
    unsigned char str[128] = {};
    int err = 0;

    /* 获取输入的地址 */
    printf("Enter the write addr: ");
    w_addr = get_uint();

    /* 判断地址 */
    if (w_addr > 256) {
        printf("err addr\r\n");
        return;
    }

    printf("Enter the write data:\n\r");
    /* 获取输入的字串 */
    gets(str);

    /* 把值写入at24cxx */
    err = at2440_write(w_addr, str, strlen(str)+1);
    if (err) {
        printf("at2440 write err %d\r\n", err);
    }
}



