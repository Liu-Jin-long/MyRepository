#include <func.h>
int main(int argc,char** argv)
{
    ARGS_CHECK(argc,3)
    int sfd=socket(AF_INET,SOCK_STREAM,0);
    printf("sfd=%d\n",sfd);
    ERROR_CHECK(sfd,-1,"socket")

    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);
    socklen_t addr_len=sizeof(server_addr);
    int ret =connect(sfd,(struct sockaddr*)&server_addr,addr_len);
    ERROR_CHECK(ret,-1,"connect");

    fd_set rdset;
    char buf[128]={0};
    printf("请输入你的姓名：\n");
    scanf("%s",buf);
    ret=send(sfd,buf,strlen(buf),0);
    ERROR_CHECK(ret,-1,"send")
    while(1)
    {
        FD_ZERO(&rdset);
        FD_SET(STDIN_FILENO,&rdset);
        FD_SET(sfd,&rdset);
        ret=select(sfd+1,&rdset,NULL,NULL,NULL);
        ERROR_CHECK(ret,-1,"select")
        if(FD_ISSET(STDIN_FILENO,&rdset))
        {
            bzero(buf,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));
            ERROR_CHECK(ret,-1,"read")
            if(!ret)
            {
                printf("我方断开。。。\n");
                break;
            }
            ret=send(sfd,buf,strlen(buf)-1,0);
            ERROR_CHECK(ret,-1,"send")
        }
        if(FD_ISSET(sfd,&rdset))
        {
            bzero(buf,sizeof(buf));
            ret=recv(sfd,buf,sizeof(buf),0);
            if(!ret)
            {
                printf("server断开。。。\n");
                break;
            }
            ERROR_CHECK(ret,-1,"recv")
            printf("%s\n",buf);

        }
    }

    close(sfd);
    return 0;

}