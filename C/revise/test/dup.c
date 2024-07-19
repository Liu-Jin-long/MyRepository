#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/mman.h>
#include <string.h>
int main()
{
    int fd=open("file",O_RDWR);
    printf("fd=%d\n",fd);//3
    //close(STDOUT_FILENO);
    int fd0=100;
    dup2(STDOUT_FILENO,fd0);
    int fd1=dup2(fd,STDOUT_FILENO);
    printf("fd1=%d\n",fd1);//2 wenjian
    close(fd);
    printf("the out of stdout 1\n");
    dup2(fd0,STDOUT_FILENO);
    printf("the out of stdout 2\n");
    return 0;
}