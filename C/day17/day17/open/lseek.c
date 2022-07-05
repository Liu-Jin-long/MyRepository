#include <func.h>
//使用lseek定位文件光标
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    off_t ret;
    ret=lseek(fd,5,SEEK_SET);//相对于开头偏移5个字节
    ERROR_CHECK(ret,-1,"lseek");
    write(fd,"xiongda",7);
    close(fd);
    return 0;
}