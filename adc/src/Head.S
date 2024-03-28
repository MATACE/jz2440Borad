/**
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

/* 异常向量表 */
_start:
    b reset             /* 0x00000000 Reset */
    ldr pc, und_addr    /* 0x00000004 Undefined instruction */
    ldr pc, swi_addr    /* 0x00000008 Software Interrupt */
    b halt              /* 0x0000000C Abort (prefetch)*/
    b halt              /* 0x00000010 Abort (data) */
    b halt              /* 0x00000014 Reserved */
    ldr pc, irq_addr    /* 0x00000018 IRQ*/
    b halt              /* 0x0000001C FIQ*/

und_addr:
    .word do_und

swi_addr:
    .word do_swi

irq_addr:
    .word do_irq

do_und:
    /* 设置und异常模式下的sp(C函数的栈) */
    ldr sp, =0x34000000
    /* 保存现场 */
    /* 在und异常模式下函数可能会修改r0-r12的数据, 先提前保存 */
    stmdb sp!, {r0-r12, lr}
    /* 处理und异常 */  
    mrs r0, cpsr
    ldr r1, =und_string
    bl info_exception
    /* 恢复现场 */
    /* ^会把spsr的值恢复到cpsr里 */
    ldmia sp!, {r0-r12, pc}^ 

und_string:
    .string "undefined instruction exception"

.align 4

do_swi:
    /* 设置swi异常模式下的sp(C函数的栈) */
    ldr sp, =0x33e00000
    /* 保存现场 */
    /* 在swi异常模式下函数可能会修改r0-r12的数据, 先提前保存 */
    stmdb sp!, {r0-r12, lr}  
    /* 保存lr的值给swi */
    mov r4, lr

    /* 处理swi异常 */
    mrs r0, cpsr
    ldr r1, =swi_string
    bl info_exception

    sub r0, r4, #4
    bl info_swi_exception

    /* 恢复现场 */
    /* ^会把spsr的值恢复到cpsr里 */
    ldmia sp!, {r0-r12, pc}^

swi_string:
    .string "swi exception"

.align 4

do_irq:
    /* 设置irq异常模式下的sp(C函数的栈) */
    ldr sp, =0x33d00000
    /* 保存现场 */
    /* 在irq异常模式下函数可能会修改r0-r12的数据, 先提前保存 */
    sub lr, lr, #4
    stmdb sp!, {r0-r12, lr}  
    /* 处理irq异常 */
    bl irq_handle
    /* 恢复现场 */
    /* ^会把spsr的值恢复到cpsr里 */
    ldmia sp!, {r0-r12, pc}^  

reset:
    /* 关闭看门狗 */
    ldr r0, =0x53000000
    ldr r1, =0
    str r1, [r0]

    /* 设置分频 */
    /* 设置MPLL, FCKL : HCLK : PCLK = 400m : 100m : 50m */
    /* LOCKTIME(0x4C000000) = 0xFFFFFFFF */
    ldr r0, =0x4C000000
    ldr r1, =0xFFFFFFFF
    str r1, [r0]

    /* CLKDIVN(0x4C000014) = 0x5, FCLK:HCLK:PCLK = 8:4:1 */
    ldr r0, =0x4C000014
    ldr r1, =0x5
    str r1, [r0]

    /* 设置CPU时钟来源 */
    /* 设置CPU工作异步模式 MMU_SetAsyncBusMode */
    mrc p15,0,r0,c1,c0,0
    orr r0,r0,#0xc0000000   
    mcr p15,0,r0,c1,c0,0

    /* 设置锁相环寄存器 */
    /* MPLLCON(0x4C000004) = (92<<12)|(1<<4)|(1<<0) 
     *  m = MDIV + 8 = 92 + 8 = 100
     *  p = PDIV + 2 = 1 + 2 = 3
     *  s = SDIV = 1
     *  FCLK = 2*m*Fin(晶振频率)/(p*2^s) = 2 * 100 * 12 / (3*2^1) = 400M
     */
    ldr r0, =0x4C000004
    ldr r1, =(92<<12)|(1<<4)|(1<<0)
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

    /* 初始化sdram */
    bl sdram_init

    /* ------使用c来拷贝和清除bss段------ */

    /* 执行时重定位text段, rodata段, data段到0x30000000的sdram */
    /* 程序从0x00000000执行 */
    @ mov r0, #0
    /* 程序第一条指令运行时的起始地址 */
    @ ldr r1, _start
    /* 程序指令运行时的结束地址,不需要复制Bss段, Bss段的起始位置*/
    @ ldr r2, =bss_start
    @ sub r2, r2, r1

    /* 复制bin文件中的数据段到执行时的地址 */
    /* 通过链接脚本获取cpy2sdram需要的参数地址 */
    /* src, dest, len */
    bl copy2sdram

    /* 清除bss段的数据 */
    /* bss段在bin文件中的加载地址 */
    @ ldr r0, =bss_start
    /* bss段在执行时的重定位地址，运行时的起始地址 */
    @ ldr r1, =bss_end

    /* 清除执行时的Bss段的地址 */
     /* 通过链接脚本获取bss_clean需要的参数地址 */
    /* start, end */
    bl clean_bss

    /** 0x00000000执行时处于svc模式
     *  现在,切换到正常的usr模式执行
     */
    /* 读出cpsr */
    mrs r0, cpsr 
    /* 修改M4-M0为0b10000,进入usr模式 */
    bic r0, r0, #0xf
    /* 清除M7,使能响应中断 */     
    bic r0, r0, #(1<<7)  
    msr cpsr, r0

    /* 设置usr模式下的栈,进入usr模式下执行 */
    ldr sp, =0x33f00000

    /* 用绝对跳转到复制代码的sdram中去执行 */
    ldr pc, =sdram
sdram:
    bl uart0_init

    bl print1

und_code:
    .word 0xeeadc0de
    bl print2

    /* 进入swi异常处理 */
    swi 0x123 

    /* 初始化中断控制器 */
    @ bl interrupt_control_init

    /* 按键外部中断初始化,设为中断源 */
    @ bl interrupt_key_init

    /* 代码没有搬移到sdram上时,只能使用bl相对跳转 */
    /* 调用main函数 使用BL命令相对跳转,程序仍然在NOR/sram执行*/
    @ bl main
    /* 代码搬移到sdram上时,绝对跳转,跳转到SDRAM执行 */
    /** 
     * 代码未搬移到sdram上时,不可使用绝对地址,程序汇编使用相对跳转pc+offset到绝对地址上访问数据
     * 不可访问完全局变量/静态变量
     * 不可访问数组(rodata, data)
     */
    ldr pc, =main

    /* 死循环执行 */
halt:
    b halt

