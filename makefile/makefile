DEMO = demo
# 获取所有的.c文件
SRC := $(wildcard ./src/*)
# 获取DEMO所有的依赖文件
# patsust取出所有.c结尾的值替换成.o结尾的值
OBJS := $(patsubst %.c, %.o, $(SRC))
# 获取所有的.h文件
INC := -I ./include
GCC := gcc
FLAG := -g -Wall
LIBS :=

# Makefile函数
NAME = a b c
NAME_O = $(foreach val, $(NAME), $(val).o)

# 即时变量
A := val1  #在定义时确定值
# 延时变量
B = val2   #在使用时确定值
C += val3  #附加,它是即时变量还是延时变量取决于前面的定义
D ?= val4  #延时变量,第一次定义才起效,在前面该变量已定义则忽略这句

# @echo 不现实Linux的shell命令

# $@ 表示目标
# $< 表示第一个依赖
# $^ 表示所有依赖

$(DEMO):main.o
	$(GCC) -o $@ $^
# 通配符
%.o:%.c
	$(GCC) $(FLAG) $(INC) -o $@ -c $< $(LIBS)

# 伪目标
.PHONY:clean
clean:
	rm -rf *.o $(DEMO)
