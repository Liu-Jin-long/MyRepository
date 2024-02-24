out:=client
SRCS:=$(wildcard ../src/*.c) #拿到目录下所有的c文件
OBJS:=$(patsubst ../src/%.c,%.o,$(SRCS))
#预定义变量
CC:=gcc
CFLAGS:=-I /usr/include/mysql -I ../include -g -D DEBUG
LDFLAGS:= -pthread -lmysqlclient -lcrypt
all:$(out)
$(out):$(OBJS) main.o
	$(CC) $^ -o $@ $(LDFLAGS)
%.o:../src/%.c
	$(CC) $(CFLAGS) -c $< -o $@
main.o:main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
#.PHONY:clean rebuild
rebuild:clean $(out)
clean:
	rm -rf $(OBJS) $(out) main.o
