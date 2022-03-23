/* bin文件的代码段的运行时地址在0x00000000,数据段位置定位到sdram的0x30000000上 */
/* 分体式脚本，用于单片机的flash和内存分开 */

SECTIONS {
    . = 0x00000000;
    . = ALIGN(4);
    /* 指定代码段的位置 */
    .text 0 : {*(.text)}
    /* 重定位数据段位置到sdram的起始位置 */
    /* AT设置bin文件中的数据段的位置 */
    /* 0x30000000程序运行时的位置 */
    /* 0x800程序在bin文件中存放的位置 */

    /* 不可以修改的内容 */
    . = ALIGN(4);
    .rodata : {*(.rodata)}

    . = ALIGN(4);
    .data 0x30000000 : AT(0x1000)  
    {
        /* 获取某个段名的位置 */
        data_load_addr = LOADADDR(.data);
        data_start = .;
        *(.data)
        data_end = .;
    }

    . = ALIGN(4);
    .bss :    
    {
        bss_start = .;
        *(.bss)
        /* 没有初值的段 */
        *(.COMMON)
        bss_end = .;
    }
}