#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

#define KEY0_VALUE 0xff // 按键值
#define KEY0_INVALID_VALUE 0x00 // 无效的按键值

#define ARGS_CHECK(argc, num) {if(argc != num){printf("file:%s line:%d  error args\n", __FILE__, __LINE__);return -1;}}
#define ERROR_CHECK(ret, retval, func_name) {if(ret == retval){printf("file:%s line:%d  errno=%d,", __FILE__, __LINE__, errno);fflush(stdout);perror(func_name);return -1;}}
int main(int argc,char** argv)
{
    ARGS_CHECK(argc, 2);
    char *filename=argv[1];
    int fd=open(filename,O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    unsigned char key_value;
    int ret;
    while (1)
    {
        ret=read(fd, &key_value, sizeof(key_value));
        ERROR_CHECK(ret,-1,"read");
        if(key_value==KEY0_VALUE)
        {
            printf("KEY0 Press, value = %#X\n",key_value);
        }
    }
    ret = close(fd);
    ERROR_CHECK(ret,-1,"close");
    return 0;
}