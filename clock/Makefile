GCC := arm-linux-gcc
CFLAGS := -Wall -g
LIBS :=
OBJS := $(patsubst %.c, %.o, $(wildcard ./*.c))
TARGET := clock

Head:Head.o $(OBJS)
	arm-linux-ld -Ttext 0 Head.o $(OBJS) -o $(TARGET).elf
	# 生成bin文件
	arm-linux-objcopy -O binary -S $(TARGET).elf $(TARGET).bin
	# 生成反汇编文件
	arm-linux-objdump -D $(TARGET).elf > $(TARGET).dis
%.o:%.c 
	$(GCC) $(CFLAGS) -o $@ -c $< $(LIBS)
%.o:%.S
	$(GCC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf *.o  *.elf *.bin *.dis
