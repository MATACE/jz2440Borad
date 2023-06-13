#include "i2c_2440_controler.h"

static P_I2C_MSG p_send_msg = NULL;

static I2C_CONTROL s3c2440_i2c_controller = {
    .name = "s3c2440",
    .init = s3c2440_i2c_init,
    .master_xfer = s3c2440_master_xfer,
};

int is_last_i2c_recv_data(void) 
{
    return p_send_msg->cnt == p_send_msg->len - 1 ? 1 : 0;
}

void i2c_handle_func(const unsigned int irq_no)
{
    /* 获取IICSTAT的值 */
    unsigned int val = IICSTAT;
    /* 传送的数值进行加 */
    p_send_msg->cnt++;
    /* 获取数据的index */
    int index = 0;

    /* 每传输完8bit一个数据,产生一个中断 */

    /* 对于每次传输,产生第1个中断时,已经发送了从机设备地址出去 */

    /* 判断时i2c的读取操作还是写入操作 */
    if (p_send_msg->flags == 0) {
        /* 写操作 */
        /**
         * 对于第一个中断,它是发送出设备地址后产生的
         * 需要判断是否有ACK
         * 有ACK:设备存在
         * 无ACK:无设备, 出错, 直接结束传输 
         */

        /* 判断第一次中断 */
        if (p_send_msg->cnt == 0) {
            /* 从i2c的状态寄存器中获取ACK的信息 */
            if (val & 1) {
                /* ACK was not received */
                /* 停止i2c传输 */
                /* Write 0xD0 (M/TStop) to IICSTAT */
                IICSTAT = 0xd0;
                /* 清除i2c的pending bit */
                /* IICCON bit4 pending bit*/
                IICCON &= ~(1 << 4);
                /* 设备不存在时外部的while循环需要退出 */
                p_send_msg->err = -1;
                printf("Tx Err, No Ack\n\r");
                /* 等待寄存器信号发出 */
                delay(1000);
                return;
            }  
        }

        /*设置需要发送的数据*/
        /* 第一个中断后的其他中断,要继续发送下一个数据 */
        if (p_send_msg->cnt < p_send_msg->len) {
            /* ACK was received */
            /* 发送下一个数据 */
            /* Write new data transmitted to IICDS */
            IICDS = p_send_msg->buf[p_send_msg->cnt];
            /* 清除i2c的pending bit */
            /* IICCON bit4 pending bit*/
            IICCON &= ~(1 << 4);
        } else {
            /* 停止传输数据 */
            /* ACK was not received */
            /* 停止i2c传输 */
            /* Write 0xD0 (M/TStop) to IICSTAT */
            IICSTAT = 0xd0;
            /* 清除i2c的pending bit */
            /* IICCON bit4 pending bit*/
            IICCON &= ~(1 << 4);

            //TODO: 是否要设置, 此时的 p_send_msg->cnt == p_send_msg->len
            /* 设备不存在时外部的while循环需要退出, 当前正常退出, 不是错误 */
            // p_send_msg->err = -1;

            /* 等待寄存器信号发出 */
            delay(1000);
            return;
        }      
    } else if (p_send_msg->flags == 1) {
        /* 读操作 */
        /**
         * 对于第一个中断,它是发送出设备地址后产生的
         * 需要判断是否有ACK
         * 有ACK:设备存在,恢复i2c传输,在下一个中断才得才能得到第一个数据
         * 无ACK:无设备, 出错, 直接结束传输 
         */
        if (p_send_msg->cnt == 0) {
            /* 从i2c的状态寄存器中获取ACK的信息 */
            if (val & 1) {
                /* ACK was not received */
                /* 停止i2c传输 */
                /* Write 0xD0 (M/TStop) to IICSTAT */
                IICSTAT = 0x90;
                /* 清除i2c的pending bit */
                /* IICCON bit4 pending bit*/
                IICCON &= ~(1 << 4);
                /* 设备不存在时外部的while循环需要退出 */
                p_send_msg->err = -1;
                /* 等待寄存器信号发出 */
                delay(1000);
                return;
            } 
            #if 0
            else {
                if (is_last_i2c_recv_data()) {
                /* 设置不需要ACK */
                /* 关闭ACK, 恢复IIC操作 */
                IICCON &= ~((1<<7) | (1<<4));

                } else {
                    /* 设置需要ACK */
                    /* 使能ACK, 恢复IIC操作 */
                    IICCON |= (1<<7); /* 回应ACK */
                    IICCON &= ~(1<<4);
                }
                return;
            }
            #endif
        }
        /**
         * 获取一个数据
         * 从IICDS读出, 保存
         */
        /* 第一个数据p_send_msg->cnt=0时舍弃,数据从1到p_send_msg->len */
        if (p_send_msg->cnt <= p_send_msg->len) {
            index = p_send_msg->cnt - 1;
            /* 从寄存器中获取数据 */
            if (index != -1) {
                /* 舍弃产生第一个中断的数据,从0到 */
                /* 当前的IICDS数据是当前中断产生以前在寄存器中的值 */
                p_send_msg->buf[index] = IICDS;
            }
            /* 恢复i2c获取数据的传输 */
            // IICCON &= ~(1 << 4);

            /* 根据at24cxx的i2c的read的时序图,最后一个数据不需要no ack */
            /* 判断当前获取的数据是否是最后获取的一个数据 */
            if (is_last_i2c_recv_data()) {
                /* 设置不需要ACK */
                /* 关闭ACK, 恢复IIC操作 */
                IICCON &= ~((1<<7) | (1<<4));

            } else {
                /* 设置需要ACK */
                /* 使能ACK, 恢复IIC操作 */
                IICCON |= (1<<7); /* 回应ACK */
	            IICCON &= ~(1<<4);
            }
        } else {
            /* 获取到所有的数据 */
            /* Write 0x90 (M/RStop) to IICSTAT */
            IICSTAT = 0x90;
            /* 清除i2c的pending bit */
            /* IICCON bit4 pending bit*/
            IICCON &= ~(1 << 4);
            /* 等待寄存器信号发出 */
            delay(1000);
        }
    }
}

int do_master_tx(P_I2C_MSG msg) 
{
    /* 保存当前的msg */
    p_send_msg = msg;

    /* 传输数值的次数,初始值从-1开始,中断中先cnt先++ */
    /* 第一个中断是在发送了从机设备地址之后产生的 */
    msg->cnt = -1;
    /* 传输中的错误值 */
    msg->err = 0;

    /* 设置寄存器启动传输 */
    /* 芯片手册的Master/Transmitter Mode逻辑框图 */
    /* 1.配置为master tx mode */
    /* In Tx mode, the IICSDA is free in the ack time */
    /* 发送模式下, 在ACK周期释放SDA */
    IICCON |= (1 << 7);
    /* Set IICSTAT to enable Serial Output */
    IICSTAT = (1<<4);

    /* 2.把从设备地址写入IICDS */
    IICDS = (msg->addr) << 1 | (0 << 0);

    /* 3.IICSTAT = 0xf0,数据被发送出去,产生i2c中断 */
    IICSTAT = 0xf0;

    /* 继续传输由中断控制器驱动 */

    /* 循环等待中断处理完成 */
    while (!msg->err && msg->cnt != msg->len);

    /* 没有设备错误 */
    if (msg->err) {
        return -1;
    }

    return 0;

}

int do_master_rx(P_I2C_MSG msg)
{
    /* 保存当前的msg */
    p_send_msg = msg;

    /* 传输数值的次数,初始值从-1开始,中断中先cnt先++ */
    /* 第一个中断是在发送了从机设备地址之后产生的 */
    msg->cnt = -1;
    /* 传输中的错误值 */
    msg->err = 0;

    /* 设置寄存器启动传输 */
    /* 芯片手册的Master/Transmitter Mode逻辑框图 */
    /* 1.配置为master tx mode */
    /* In Rx mode, the IICSDA is L in the ack time */
    /* 发送模式下, 在ACK周期回应ACK */
    IICCON |= (1 << 7);
    /* Set IICSTAT to enable Serial Output */
    IICSTAT = (1<<4);

    /* 2.把从设备地址写入IICDS */
    IICDS = (msg->addr) << 1 | (1 << 0);

    /* 3.IICSTAT = 0xb0,数据被接受到,产生i2c中断 */
    IICSTAT = 0xb0;

    /* 继续传输由中断控制器驱动 */

    /* 循环等待中断处理完成 */
    while (!msg->err && msg->cnt != msg->len+1);

    /* 没有设备错误 */
    if (msg->err) {
        return -1;
    }

    return 0;

}

int s3c2440_i2c_init(void)
{
    /* 配置i2c的GPIO管脚 */
    GPECON &= ~((3 << 30) | (3 << 28));
    GPECON |= (2 << 30) | (2 << 28);

    /* 设置i2c的clk时钟 */
    /**
     * bit7: 1 enable in rx mode
     * bit6: 时钟源 0:IICCLK = fPCLK/16, 1:IICCLK = fPCLK/512
     * bit5: 1:Enable
     * bit4: 读出为1时表示中断发生了,写入0来清除并恢复i2c操作
     * bit3-bit0: Tx clock = IICCLK/(IICCON[3:0]+1)
     * Tx clock = 100khz = 50Mhz/16/(IICCON[3:0]+1)
     * IICCON[3:0] = 30.25
     * 
     */

    IICCON =  (1 << 7) | (0 << 6) | (30 << 0) | (1 << 5);

    /* 注册中断处理函数 */
    register_irq_func(27, i2c_handle_func);

    return 0;

}

int s3c2440_master_xfer(P_I2C_MSG msgs, int num)
{
    int err = 0;

    /* 发送每个i2c_msg数据 */
    for (unsigned int i = 0; i < num; i++) {
        if (msgs[i].flags == 0) {
            /* 写操作 */
            err = do_master_tx(&msgs[i]);
            
        } else if (msgs[i].flags == 1) {
            /* 读操作 */
            err = do_master_rx(&msgs[i]);
        }

        /* 判断是否传输成功 */
        if (err) {
            printf("s3c2440_master_xfer faield\r\n");
            return err;
        }
    }
    return 0;
}

int s3c2440_i2c_controller_add(void)
{
    int ret = 0;
    /* 注册当前的s3c2440控制器 */
    ret = regisger_i2c_controller(&s3c2440_i2c_controller);

    // printf("s3c2440_i2c_controller_add==%d\r\n", ret);

    return 0;
}

