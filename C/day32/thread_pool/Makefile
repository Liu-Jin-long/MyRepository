#目前是最通用的Makefile
out:=thread_pool_server
SRCS:=$(wildcard *.c) #拿到当前目录下所有的c文件
OBJS:=$(patsubst %.c,%.o,$(SRCS))
#预定义变量
CC:=gcc
CFLAGS:=-g -D DEBUG
$(out):$(OBJS)
	gcc $^ -o $@ -pthread

#.PHONY:clean rebuild
rebuild:clean $(out)
clean:
	rm -rf $(OBJS) $(out)
