#include <func.h>
//使用mmap对文件内容进行修改
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    int ret;
    char *p;//mmap成功后返回一个堆内存的起始地址
    p=(char*)mmap(NULL,5,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(p,(char*)-1,"mmap");
    printf("%s\n",p);
    p[0]='H';
    ret=munmap(p,5);//解除映射，解除映射后，DMA会把数据回写到磁盘上
    ERROR_CHECK(ret,-1,"munmap");
    return 0;
}