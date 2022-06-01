#include "dht11.h"

void set_dth11_gpio_mode(DTH_MODE mode)
{
    /* 设置dht11的GPIO的模式 */
    if (mode == DTH_O) {
        /* 设置成输出引脚模式 */
        GPGCON &= ~(3 << 10);
        GPGCON |= (1 << 10);

    } else if (mode == DTH_I) {
        /* 设置成输入引脚模式 */
        GPGCON &= ~(3 << 10);
    }
}

void set_dth11_gpio_pin(DTH_PIN pin)
{
    /* 设置dht11的GPIO的输出电平 */
    if (pin == DTH_H) {
        GPGDAT |= 1 << 5;
    } else if (pin == DTH_L) {
        GPGDAT &= ~(1 << 5);
    }
}

DTH_PIN get_dht11_gpio_pin(void)
{
    if (GPGDAT & (1 << 5)) {
        return DTH_H;
    } else {
        return DTH_L;
    }
}

void dth11_init(void)
{
    /* 设置成GPIO输出功能 */
    set_dth11_gpio_mode(DTH_O);
    /* 设置成高电平 */
    set_dth11_gpio_pin(DTH_H);
    /* 上电时的不稳定状态,等待2s */
    mdelay(2000);
    return;
}

int dht11_wait_ack_pin(DTH_PIN val, int time_out)
{
    /* 等待ACK和超时时间 */
    while (time_out--) {
        if (val == get_dht11_gpio_pin()) {
            return 0;
        }
        udelay(1);
    }
    return -1;
}


void dht11_start(void)
{
    /* 设置引脚为输出引脚 */
    set_dth11_gpio_mode(DTH_O);
    /* 拉低电平 */
    set_dth11_gpio_pin(DTH_L);
    /* 至少延时18ms */
    mdelay(20);

    /* 拉高电平 */
    // set_dth11_gpio_pin(DTH_H);
    /* 设置成输入引脚,通过上拉电阻拉高电平 */
    set_dth11_gpio_mode(DTH_I);

    /* 至少延时20us~40us */
    // udelay(30);

    return;
}

int dht11_recv_byte(void)
{
    int data = 0;
    for (unsigned int i = 0; i < 8; i++) {
        /* 等待获取数据的高电平 */
        if (0 != dht11_wait_ack_pin(DTH_H, 1000)) {
            printf("Recv Byte Hight Failed\r\n");
            return -1;
        }
        udelay(26);
        /* data移位 */
        data <<= 1;
        /* 根据获取的电平,来判断数据 */
        if (DTH_H == get_dht11_gpio_pin()) {
            data |= 1;
        }
        /* 等待数据拉低 */
        if (0 != dht11_wait_ack_pin(DTH_L, 1000)) {
            printf("Recv Byte Low Failed %d\r\n", i);
            return -1;
        }
    }

    return data;

}

int dht11_ready_read(int *hum, int *temp)
{
    unsigned char hum_int = 0;
    unsigned char hum_dec = 0;
    unsigned char temp_int = 0;
    unsigned char temp_dec = 0;
    unsigned char check = 0;

    /* 主机发送DHT11的起始信号 */
    dht11_start();

    /* 主机延时等待接受数据 */
    /* 等待DHT响应信号 */
    if (0 != dht11_wait_ack_pin(DTH_L, 1000)) {
        printf("Wait Start DHT Ack Failed\r\n");
        return -1;
    }
    
    /* 等待DHT拉高信号 */
    if (0 != dht11_wait_ack_pin(DTH_H, 1000)) {
        printf("Wait Start DHT For Hight Failed\r\n");
        return -1;
    }

    /* 等待DHT传输数据信号 */
    if (0 != dht11_wait_ack_pin(DTH_L, 1000)) {
        printf("Wait Start DHT For Data Failed\r\n");
        return -1;
    }

    /* 接受40bit的数据 */
    hum_int = dht11_recv_byte();
    hum_dec = dht11_recv_byte();
    temp_int = dht11_recv_byte();
    temp_dec = dht11_recv_byte();
    check = dht11_recv_byte();

    /* 等待从机拉低时间, 等待100us*/
    udelay(100);
    
    /* 恢复主机拉高总线 */
    set_dth11_gpio_mode(DTH_O);
    set_dth11_gpio_pin(DTH_H);
    printf("%d %d %d %d %d\r\n", hum_int, hum_dec, temp_int, temp_dec, check);

    if (check == hum_int + hum_dec + temp_int + temp_dec) {
        *hum = hum_int;
        *temp = temp_int;
        return 0;
    } else {
        *hum = 0;
        *temp = 0;
        return -1;
    }

    return -1;
}













