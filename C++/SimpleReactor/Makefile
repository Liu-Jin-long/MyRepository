out:=server
#SRCS:=$(wildcard *.hpp)
#预定义变量
CC:=g++
CFLAGS:= -g -D DEBUG -std=c++11
LDFLAGS:= -pthread
all:$(out)
$(out):main.cpp
	$(CC) $^ -o $@ $(LDFLAGS)
.PHONY:clean rebuild
rebuild:clean $(out)
clean:
	rm -rf $(out)

