#include <func.h>
//2号聊天，先打开一号管道写端，再打开二号管道读端
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3)
    int fdw,fdr;
    fdw=open(argv[1],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");

    fdr=open(argv[2],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");
    printf("char2:\n");


    char buff[64]={0};
    fd_set r_set;//fd集合其实是个数组
    int ret;
    int ready_fd;

    while(1)
    {
        
        FD_ZERO(&r_set);
        FD_SET(STDIN_FILENO,&r_set);
        FD_SET(fdr,&r_set);
        ready_fd=select(fdr+1,&r_set,NULL,NULL,NULL);
        ERROR_CHECK(ready_fd,-1,"select")
        
    
        if(FD_ISSET(STDIN_FILENO,&r_set))
        {
            memset(buff,0,sizeof(buff));
            ret=read(STDOUT_FILENO,buff,sizeof(buff));
            ERROR_CHECK(ret,-1,"read")
            if(0==ret)
            {
                printf("leave\n");
                break;
            }
            ret=write(fdw,buff,strlen(buff)-1);//read时默认会读到\n,write时去除
            ERROR_CHECK(ret,-1,"write")
        }
        
        

        if(FD_ISSET(fdr,&r_set))
        {
            memset(buff,0,sizeof(buff));
            ret=read(fdr,buff,sizeof(buff));
            ERROR_CHECK(ret,-1,"read")
            if(0==ret)//写端断开
            {
                printf("Writing has been disconnected\n");
                break;
            }
            printf("char2 say: %s\n",buff);
        }   

    }
        
        


    close(fdr);
    close(fdw);
    return 0;
}