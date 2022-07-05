#include <func.h>
//一用户编写一个程序，用来往file文件中写入内容hello，然后编译后，该用户给可执行文件增加s权限，这时另一执行该程序，能够往file文件中写入hello

int main()
{
    int fd=open("file",O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    int ret=write(fd,"hello",5);
    ERROR_CHECK(ret,-1,"write");
    return 0;
    //chmod u+s filename权限提升
    //chmod 4777 filename

}