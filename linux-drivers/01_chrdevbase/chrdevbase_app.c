#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
 #include <stdlib.h>
#define ARGS_CHECK(argc, num) {if(argc != num){printf("file:%s line:%d  error args\n", __FILE__, __LINE__);return -1;}}
#define ERROR_CHECK(ret, retval, func_name) {if(ret == retval){printf("file:%s line:%d  errno=%d,", __FILE__, __LINE__, errno);fflush(stdout);perror(func_name);return -1;}}
int main(int argc,char** argv)
{
    ARGS_CHECK(argc, 3);
    char *filename=argv[1];
    char readbuf[100], writebuf[100]="user data";

    int fd=open(filename,O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    ssize_t size=0;
    if(atoi(argv[2])==1)//读
    {
        size=read(fd,readbuf,50);
        ERROR_CHECK(size,-1,"read");
        printf("从设备中读取readbuf=%s\n",readbuf);
    }else if(atoi(argv[2])==2)//写
    {
        size=write(fd,writebuf,50);
        ERROR_CHECK(size,-1,"write");
    }
    int ret=close(fd);
    ERROR_CHECK(ret,-1,"close");
    return 0;
}