#目标:依赖（多个依赖文件用空格隔开）
#<Tab键规则>命令

#目标文件一定要存在，依赖文件可以不写
#自动化变量 $^代表所有的依赖文件  $@代表目标文件 $<代表第一个依赖文件

#自定义变量 默认字符串型
#命名规则和C语言相同、大小写敏感、不使用#
#引用变量和shell编程相同，加上符号$，如果访问不成功，就给变量加上括号
#Makefile不是shell脚本，等号两边可以有空格
#所有的变量类型都是字符串，使用的时候不需要加""
#系统变量环境：有些变量是系统预设，可以直接使用
#常用：CC编译器的名字 默认CC等于gcc 可自行改动例如CC = arm-linux-gcc  RM删除文件 等于rm -f

#伪指令 当使用(RM)：make clean出现删除检查提示已最新的错误
#可能删不掉，只约束清空删除检查需要再clean上面加上一个伪指令
#伪指令 .PHONY:clean

#Makefiel函数 $(<function> <arguments>,<arguments>,<arguments>)
# $(wildcard )作用：在某一路径下寻找对应匹配文件  
# $(patsubst %.c,%.o,依赖文件)模式字符串替换函数作用：将所有的*.c文件名字变成为*.o文件 相同作用见下面第36行的另一种形式

#多目录工程
#源文件目录   src      放置*.c文件
#头文件目录   include  放置*.h头文件
#库文件目录   lib      放置*.so文件
#可执行文件   bin      最终生成的二进制可执行文件
#  -I  头文件目录连接  指定头文件路径
#  -i  库文件访问      指定头文件名字
#  -L  库文件目录连接  指定连接的动态库或者静态库路径
#  -l  库文件访问      指定需要链接的库的名字（链接 libc.a : -lc 链接动态库：libc.so : -lc） 注意：-l后面可以直接添加库名省去“lib”和“.so”或“.a”

#目前是最通用的Makefile
CC:=gcc
SRCS:=$(wildcard *.c) #拿到当前目录下所有的c文件
TARGETS:=$(SRCS:%.c=%)
all:$(TARGETS)
%:%.c
	$(CC) -g $< -o $@ -lpthread
rebuild: clean all
clean:
	rm -rf $(TARGETS)
