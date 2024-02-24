#include <func.h>
//改变文件大小
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3)
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    int ret;
    ret=ftruncate(fd,atoi(argv[2]));
    ERROR_CHECK(fd,-1,"ftruncate");
    return 0;
}