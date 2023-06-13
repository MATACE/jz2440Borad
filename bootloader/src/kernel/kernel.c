#include "kernel.h"

#define MACH_TYPE_S3C2440 362

static struct tag *params;

void kernel_menu(void)
{
    unsigned char ch = ' ';
    
    while (1) {
        /* 输出菜单 */
        printf("[k] Run With Kernel\n\r");
        printf("[q] Quit\n\r");
        printf("Enter Selection: ");

        /* 获取字符 */
        ch = getchar();
        printf("%c\n\r", ch);

        switch (ch) {
            case 'k':
                do_kernel();
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

void setup_start_tag(void)
{
    /* 0x30000100存放参数的位置 */
    params = (struct tag *)(0x30000100);

    params->hdr.tag = ATAG_CORE;
    params->hdr.size = tag_size(tag_core);

    params->u.core.flags = 0;
    params->u.core.pagesize = 0;
    params->u.core.rootdev = 0;

    params = tag_next(params);
}

void setup_memory_tag(void)
{
    params->hdr.tag = ATAG_MEM;
    params->hdr.size = tag_size(tag_mem32);

    /* 内存的起始地址 */
    params->u.mem.start = 0x30000000;
    /* 内存的总共的大小 */
    params->u.mem.size = 0x4000000;

    params = tag_next(params);
}

void setup_commandline_tag(char *cmdline)
{
    unsigned int len = strlen(cmdline) + 1;

    params->hdr.tag = ATAG_CMDLINE;
    /* 向四取整 +3 >> 2 */
    params->hdr.size =
    (sizeof(struct tag_header) + len + 3) >> 2;

    strcpy(params->u.cmdline.cmdline, cmdline);

    params = tag_next(params);
}

void setup_end_tag(void)
{
    params->hdr.tag = ATAG_NONE;
    params->hdr.size = 0;
}

void do_kernel(void)
{
    void (*theKernel)(int zero, int arch, unsigned int params);

    /* 内核启动时没有启动串口, 帮内核启动串口, 打印串口输出内容 */
    

    /* uboot划分flash  */
    /**
     *  device nand0 <nandflash0>, # parts = 4
     *  #: name                size            offset          mask_flags
     *  0: bootloader          0x00040000      0x00000000      0
     *  1: params              0x00020000      0x00040000      0
     *  2: kernel              0x00200000      0x00060000      0
     *  3: root                0x0fda0000      0x00260000      0
     * 
     */
    /* 从Nand中的kernel分区读取到kernel到sdram中执行 */
    /* 从flash上的分区地址读取到kernel, 读到内存上, 读的大小 */
    /* uzmage有64字节的头, 读到0x30007FC0的位置,  读2M的大小, 在0x30008000入口执行 */
    /** 
     * 设置bootcmd=nand read.jffs2 0x30007FC0 kernel; bootm 0x30007FC0 
     */
    read_nand(0x60000 + 0x64, (unsigned char *)0x30008000, 0x200000);


    /* uboot获取uimage, 前64字节是imaged的头部, 包含加载地址和入口地址 */
    /* bootm读取加载地址和入口地址, 把移动内核到合适的地方启动 */
    /* do_bootm_linux传入参数启动内核 */
    /* 设置内核参数的位置, 设置启动参数(TAG), 跳转到入口地址启动内核 */
    /**
     * start_tag
     * memory_tags : 内存的地址和大小
     * commandline_tag : 内核第一个程序, 串口输出
     * end_tag
     */

    /**
     * 0               zero
     * bi_arch_number 机器ID,内核支持单板
     * bi_boot_params 参数存放的地址
     * heKernel (0, bd->bi_arch_number, bd->bi_boot_params);
     * 
     */
    

    /* 向内核传递参数 */
    setup_start_tag();
    setup_memory_tag();
    /* 设置bootargs=noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0 */
    setup_commandline_tag("noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0");
    setup_end_tag();

    /* 跳转执行到内核代码 */
    /* 内核的函数入口存放在0x30008000 */
    theKernel = (void (*)(int, int, unsigned int))0x30008000;
    theKernel(0, MACH_TYPE_S3C2440, 0x30000100);

    /* 内核执行正常, 不执行到uboot代码这里 */
    printf("Uboot Do Kernel Failed ##############\r\n");
    
}


