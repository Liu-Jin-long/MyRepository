#include <func.h>
//做一个daemon进程，让其每2秒钟，在/tmp/log文件中写一句 Good good study,day day up.

void Daemon()
{
    if(fork())
    {
        exit(0);
    }

    setsid();
    chdir("/");
    umask(0);
    int MaxFile_fd=3;
    for(int i=0;i<MaxFile_fd;i++)
    {
        close(i);
    }
}

int main()
{
    Daemon();
    int fd=open("/tmp/log",O_WRONLY);
    ERROR_CHECK(fd,-1,"open");
    int ret;
    while(1)
    {
        ret=write(fd,"Good good study,day day up.",27);
        ERROR_CHECK(ret,-1,"write");
        sleep(2);
    }
    return 0;

}