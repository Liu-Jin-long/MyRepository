#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/mman.h>
#include <string.h>
int main()
{
    int fd=open("file",O_RDWR);
    printf("fd=%d\n",fd);
    ftruncate(fd,9);
    char* p=(char*)mmap(NULL,100,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    printf("strlen(p)=%ld\n",strlen(p));
    printf("p=%s\n",p);
    strcat(p,"qwwqqqqqqwwwwwww");
    printf("p=%s\n",p);
    char buf[100]={0};
    int ret=read(fd,buf,sizeof(buf));
    printf("buf=%s\n",buf);
    return 0;
}