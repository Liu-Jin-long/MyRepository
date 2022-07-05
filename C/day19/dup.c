#include <func.h>
//将标准输出重定向到log1文件

int main(int argc ,char* argv[])
{
    ARGS_CHECK(argc,2)
    int fd=open(argv[1],O_RDWR);
    ERROR_CHECK(fd,-1,"open");
    printf("\n");//刷新标准输出(规定)
    close(STDOUT_FILENO);
    int fd1=dup(fd);
    close(fd);
    printf("helloworld\n");
    close(fd1);
    return 0;
}