Head:
	# 编译不链接
	arm-linux-gcc -c -o Head.o Head.S
	# 链接
	arm-linux-ld -Ttext 0 Head.o -o Head.elf
	# 生成bin文件
	arm-linux-objcopy -O binary -S Head.elf Head.bin
	# 生成反汇编文件
	arm-linux-objdump -D Head.elf > Head.dis

.PHONY: clean
clean:
	rm -rf Head.o Head.elf Head.bin Head.dis
