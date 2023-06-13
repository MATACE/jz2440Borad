#ifndef _KERNEL_H
#define _KERNEL_H

#include "nandflash.h"
#include "setup.h"
#include "common.h"
#include "string_utils.h"

void setup_start_tag(void);
void setup_memory_tag(void);
void setup_commandline_tag(char *cmdline);
void setup_end_tag(void);
void kernel_menu(void);
void do_kernel(void);

#endif // _KERNEL_H

