#目前是最通用的Makefile
CC:=gcc
SRCS:=$(wildcard *.c) #拿到当前目录下所有的c文件
TARGETS:=$(SRCS:%.c=%)
all:$(TARGETS)
%:%.c
	$(CC) $< -o $@

clean:
	rm -rf $(TARGETS)
