#include <func.h>
//open时创建文件,存在时失败
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDWR|O_CREAT|O_EXCL,0666);
    ERROR_CHECK(fd,-1,"open");
    close(fd);
    return 0;
}
