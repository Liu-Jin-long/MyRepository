#include <func.h>
//做一个文件空洞
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,2)
    int fd;
    fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    off_t ret;
    ret=lseek(fd,1024,SEEK_SET);//相对于开头偏移1024个字节
    ERROR_CHECK(ret,-1,"lseek");
    write(fd,"1",1);
    close(fd);
    return 0;
}