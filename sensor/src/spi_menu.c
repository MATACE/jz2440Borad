#include "spi_menu.h"
#include "spi_gpio_menu.h"

unsigned int SPI_MODE_INIT_FLAG = 0;

void spi_menu(void)
{
    unsigned char ch = ' ';
    
    while (1) {
        /* 输出菜单 */
        printf("[1] Spi GPIO Test\n\r");
        printf("[2] Spi CONTROLER REG Test\n\r");
        printf("[q] quit\n\r");
        printf("Enter selection: ");

        /* 获取字符 */
        ch = getchar();
        printf("%c\n\r", ch);

        switch (ch) {
            case '1':
                SPI_MODE_INIT_FLAG = 0;
                spi_gpio_menu();
            break;
            case '2':
                SPI_MODE_INIT_FLAG = 1;
                spi_gpio_menu();
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


