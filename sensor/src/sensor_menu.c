#include "sensor_menu.h"
#include "dht11.h"

void sensor_menu(void)
{
    unsigned char ch = ' ';
    
    while (1) {
        /* 输出菜单 */
        printf("[1] Sensor Photoresistance Test\n\r");
        printf("[2] Sensor DTH11 Test\n\r");
        printf("[q] Quit\n\r");
        printf("Enter Selection: ");

        /* 获取字符 */
        ch = getchar();
        printf("%c\n\r", ch);

        switch (ch) {
            case '1':
                photoresistance_test();
            break;
            case '2':
                /* 精确延时函数不准确 */
                dht11_test();
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

void photoresistance_test(void)
{
    /* 注册中断 */
    photo_gpio_init();

    /* 获取光光敏电阻的值 */
    while (1) {
        get_photoresistance_val();
    }

}


void dht11_test(void)
{
    int temp = 0, hum = 0;
    /* 上电初始化 */
    dth11_init();
    
    /* 发送信号获取数据 */
    if (dht11_ready_read(&hum, &temp) != 0) {
        printf("DHT11 read failed\r\n");
    } else {
        printf("Hum %02d, temp %02d\r\n", hum, temp);
    }

}

