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
    /* 设置栈空间: sp栈 */
    /* nand启动, 片内4k内存就是零地址 */
    ldr sp, =4096
    /* nor启动,片内地址起始地址0x40000000+4k空间 */
    // ldr sp, =0x40000000+4096
    
    /* 调用main函数 */
    bl main

    /* 死循环执行 */
halt:
    b halt
        


