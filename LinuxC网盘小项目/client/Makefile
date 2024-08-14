out:=client
SRCS:=../src/client_op.c ../src/tcp_epoll_op.c ../src/transfer_file.c ../src/md5sum.c
OBJS:=$(patsubst ../src/%.c,%.o,$(SRCS))
#预定义变量
#CC:=arm-linux-gnueabihf-gcc
CC:=gcc
CFLAGS:=-I ../include -g -D DEBUG -std=c99
LDFLAGS:= -pthread -lcrypt
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
