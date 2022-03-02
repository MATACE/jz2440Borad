/*
 * ldr:读内存指令 ldr R0, [R1]
 * str:写内存命令 str R0, [R1]
 * b:跳转指令
 * mov:移动指令 mov R0, R1
 * 伪指令:ldr r0, = 0x100
 * add r0, r1, #4 -> r0 = r1 + 4
 * sub r0, r1, #4
 * bl:跳转指令
 * ldm:读内存写入多个寄存器
 * stm:把多个寄存器的值写入寄存器
 * ldmia:从栈中读取数据 先读后增
 * stmdb:从栈中存取数据 先减后存
 */

 .text
 .global _start

_start:
    /* 关闭看门狗 */
    ldr r0, =0x53000000
    ldr r1, =0
    str r1, [r0]

    /* 分辨是nor/nand启动 
     *  nor像硬盘一样读,不可以像硬盘一样写
     * 写0到0地址,读出来
     * 如果值为0,表示0写入0地址,对应是ram是nand
     * 否则是nor启动
     */
    
    /* 获取零地址的值 */
    mov r1, #0
    /* 备份零地址的值 */
    ldr r0, [r1]
    /* 往零地址写零 */
    str r1, [r1]
    /* 读取零地址值 */
    ldr r2, [r1]
    /* 比较r2的值和零 */
    cmp r1, r2
    /* r1 == r2? 如果相等Nand启动 */
    /* nor启动,片内地址起始地址0x40000000+4k空间 */
    ldr sp, =0x40000000+4096
    /* Nand启动 */
    moveq sp, #4096
    /* 恢复寄存器原来的值 */
    streq r0, [r1]

    /* 设置栈空间: sp栈 */
    /* nand启动, 片内4k内存就是零地址 */
    @ ldr sp, =4096
    /* nor启动,片内地址起始地址0x40000000+4k空间 */
    // ldr sp, =0x40000000+4096
    
    /* 调用main函数 */
    bl main

    /* 死循环执行 */
halt:
    b halt
        


