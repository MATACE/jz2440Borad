GCC := arm-linux-gcc
CFLAGS := -Wall -g -march=armv4 -std=c99
LIBS := 
HEADS := $(patsubst %.S, %.o, $(wildcard ./src/*.S))
# NANDS := $(patsubst %.c, %.o, $(wildcard ./src/nandflash4k/*.c))
OBJS := $(patsubst %.c, %.o, $(wildcard ./src/*.c))
ARM_S := $(patsubst %.S, %.o, $(wildcard ./arm_s/*.S))
TARGET := nandflashop
INCLUDE := -I ./include

Head:$(HEADS) $(OBJS) $(ARM_S)
	# Nandflash启动时前4k要负责搬移数据
	arm-linux-ld -T sdram.lds $(HEADS) $(OBJS) $(ARM_S) -o $(TARGET).elf
	# 生成bin文件
	arm-linux-objcopy -O binary -S $(TARGET).elf $(TARGET).bin
	# 生成反汇编文件
	arm-linux-objdump -D $(TARGET).elf > $(TARGET).dis
%.o:%.c 
	$(GCC) $(CFLAGS) $(INCLUDE) -o $@ -c $< $(LIBS)
%.o:%.S
	$(GCC) $(CFLAGS) $(INCLUDE) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf ./src/*.o  *.elf *.bin *.dis ./arm_s/*.o ./src/nandflash4k/*.o
