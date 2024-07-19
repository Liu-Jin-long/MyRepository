#include<signal.h>//更改"cStandard": "c11",
#include<stdio.h>
#include<string.h>
#include<unistd.h>
int main()
{
    int fds[2];
    pipe(fds);
    sigset_t setSig;
    sigemptyset(&setSig);
    sigaddset(&setSig,SIGPIPE);
    sigprocmask(SIG_BLOCK,&setSig,NULL);
    char buf[10]={0};
    int ret;
    if(fork()==0)
    {
        close(fds[1]);
        sleep(1);
        if(read(fds[0],buf,sizeof(buf)))
        {
            puts(buf);
            close(fds[0]);
        }
    }else{
        close(fds[0]);
        ret=write(fds[1],"hello",5);
        printf("ret=%d\n",ret);
        wait(NULL);
        ret=write(fds[1],"world",5);
        printf("ret=%d\n",ret);
        sleep(1);
        ret=write(fds[1],"hhhhh",5);
        printf("ret=%d\n",ret);
        close(fds[1]);
    }
    return 0;
    
}