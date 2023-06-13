GCC := arm-linux-gcc
CFLAGS := -Wall -g -march=armv4 -std=c99
LIBS :=
HEADS := $(patsubst %.S, %.o, $(wildcard ./src/*.S))
# NANDS := $(patsubst %.c, %.o, $(wildcard ./src/nandflash4k/*.c))
OBJS := $(patsubst %.c, %.o, $(wildcard ./src/*.c))
OBJS += $(patsubst %.c, %.o, $(wildcard ./src/lcd/*.c))
OBJS += $(patsubst %.c, %.o, $(wildcard ./src/i2c/*.c))
OBJS += $(patsubst %.c, %.o, $(wildcard ./src/spi/*.c))
OBJS += $(patsubst %.c, %.o, $(wildcard ./src/sensor/*.c))
OBJS += $(patsubst %.c, %.o, $(wildcard ./src/kernel/*.c))
ARM_S := $(patsubst %.S, %.o, $(wildcard ./arm_s/*.S))
TARGET := dokernel
INCLUDE := -I ./include
INCLUDE += -I ./include/lcd
INCLUDE += -I ./include/i2c
INCLUDE += -I ./include/spi
INCLUDE += -I ./include/sensor
INCLUDE += -I ./include/kernel

Head:$(HEADS) $(OBJS) $(ARM_S)
	# Nandflash启动时前4k要负责搬移数据
	arm-linux-ld -T sdram.lds $(HEADS) $(OBJS) $(ARM_S) libgcc.a -o $(TARGET).elf
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
	rm -rf ./src/*.o  *.elf *.bin *.dis ./arm_s/*.o ./src/lcd/*.o ./src/spi/*.o ./src/sensor/*.o ./src/kernel/*.o
