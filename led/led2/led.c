
#define P_GPFCON *((volatile unsigned int *)0x56000050)
#define P_GPFDAT *((volatile unsigned int *)0x56000054)

int main()
{
    /* 设置GPIO配置寄存器 */
    P_GPFCON = 0x1100;

    /* 设置GPIO管脚的电平 */
    P_GPFDAT = 0;

    return 0;
    
}