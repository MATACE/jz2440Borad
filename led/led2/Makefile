Head:
	# 编译不链接
	arm-linux-gcc -c -o led.o led.c
	arm-linux-gcc -c -o Head.o Head.S
	# 链接
	arm-linux-ld -Ttext 0 Head.o led.o -o led.elf
	# 生成bin文件
	arm-linux-objcopy -O binary -S led.elf led.bin
	# 生成反汇编文件
	arm-linux-objdump -D led.elf > led.dis

.PHONY: clean
clean:
	rm -rf *.o  *.elf *.bin *.dis
