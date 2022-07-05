#include <func.h>
//两个进程通过管道文件实现即时聊天效果

//1号聊天，先打开一号管道读端，再打开二号管道写端
int main(int argc,char* argv[])
{
    ARGS_CHECK(argc,3)
    int fdw,fdr;
    fdr=open(argv[1],O_RDONLY);
    ERROR_CHECK(fdr,-1,"open");

    fdw=open(argv[2],O_WRONLY);
    ERROR_CHECK(fdw,-1,"open");
    printf("char1:\n");

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
            //讲话
            memset(buff,0,sizeof(buff));
            ret=read(STDOUT_FILENO,buff,sizeof(buff));//读来自键盘的标准输入(写入buff)
            ERROR_CHECK(ret,-1,"read")
            if(0==ret)//标准输入Ctrl+D终止，read返回0
            {
                printf("leave\n");
                break;
            }
            write(fdw,buff,strlen(buff)-1);//写入管道(read时默认会读到\n,write时去除)
            ERROR_CHECK(ret,-1,"write")
        }

        if(FD_ISSET(fdr,&r_set))
        {
            //读对方发的数据
            memset(buff,0,sizeof(buff));
            ret=read(fdr,buff,sizeof(buff));//管道没数据，read就会阻塞
            ERROR_CHECK(ret,-1,"read")
            if(0==ret)//当写端断开关闭，读端不会阻塞，read返回0,判断防止对方退出而疯狂打印
            {
                printf("Writing has been disconnected\n");
                break;
            }
            printf("chat1 say: %s\n",buff);
        }
        
    }




    close(fdr);
    close(fdw);
    return 0;

}