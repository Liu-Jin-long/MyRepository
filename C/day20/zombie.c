#include <func.h>

//得到僵尸进程
int main()
{
    pid_t pid=fork();
    if(0==pid)
    {
        return 0;
    }else
    {
        while(1);
    }
}