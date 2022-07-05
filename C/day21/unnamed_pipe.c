#include <func.h>
//创建一个无名管道，fork子进程，然后子进程向管道写端写Hello，父进程接收数据并打印

int main()
{
    int fd[2];
    pipe(fd);
    char buff[64]={0};
    if(fork())
    {//父进程
        close(fd[1]);//关闭写
        // sleep(2);
        read(fd[0],buff,sizeof(buff));
        printf("parent process gets=%s",buff);
        wait(NULL);
        return 0;
    }else//子进程
    {
        close(fd[0]);//关闭读
        write(fd[1],"Hello",5);
        return 0;
    }
}
