/*
 * ldr:读内存指令 ldr R0, [R1]
 * str:写内存命令 str R0, [R1]
 * b:跳转指令
 * mov:移动指令 mov R0, R1
 * 伪指令:ldr r0, = 0x100
 */

 .text
 .global _start

_start:
    /* 配置LED的GPIO管脚的功能 */
    /* 获取GPIO配置基本地址 */
    ldr r1, =0x56000050
    /* 获取GPIO寄存器配置值 */
    ldr r0, =0x1500
    /* 设置内存地址值 */
    str r0, [r1]
    
    /* 配置输出高低电平 */
    ldr r1, =0x56000054
    ldr r0, =0
    str r0, [r1]

    /* 死循环执行 */
halt:
    b halt
        


