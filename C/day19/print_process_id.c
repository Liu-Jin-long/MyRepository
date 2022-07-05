#include <func.h>
//打印当前进程pid，父进程pid，真实用户ID，真实组ID，有效用户ID，有效组ID
void main()
{
    printf("pid=%d,ppid=%d\nuid=%d,gid=%d\neuid=%d,egid=%d\n",getpid(),getppid(),getuid(),getgid(),geteuid(),geteuid());
    
    while(1);//防止程序直接执行结束而关闭进程
}