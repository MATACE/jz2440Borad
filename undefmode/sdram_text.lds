/** bin文件的起始代码段在sdram的0x30000000,
 * 通过在0x00000000地址上执行,
 * 把整个bin程序重定位到sdram的0x30000000运行时执行
 */

SECTIONS {
    /* 重定位整个程序的位置到sdram的起始位置 */
    . = 0x30000000;
    code_start = .;
    
    . = ALIGN(4);
    /* 指定代码段的位置 */
    .text 0 : {*(.text)}

    /* 不可以修改的内容 */
    . = ALIGN(4);
    .rodata : {*(.rodata)}

    . = ALIGN(4);
    .data : {*(.data)}

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