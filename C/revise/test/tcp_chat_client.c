#include <func.h>
//通过select实现tcp的即时聊天
int main(int argc,char** argv)
{
    ARGS_CHECK(argc,3);
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);//套接口描述符，对应的是一个缓冲区
    ERROR_CHECK(sfd,-1,"socket");
    printf("sfd=%d\n",sfd);
    struct sockaddr_in my_addr;//Structure describing an Internet socket address.
    bzero(&my_addr,sizeof(my_addr));//要清空
    my_addr.sin_family=AF_INET;//AF_INET代表ipv4，AF_INET6代表ipv6
    my_addr.sin_addr.s_addr=inet_addr(argv[1]);//点分十进制转为网络字节序
    my_addr.sin_port=htons(atoi(argv[2]));//端口号转为网络字节序

    //客户端连接服务器
    int ret=connect(sfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
    ERROR_CHECK(ret,-1,"connect");
    
    char buf[128]={0};
    fd_set sfd_set;
    while(1)
    {
        //清空集合，写入要监视的网络套接字描述符
        FD_ZERO(&sfd_set);
        FD_SET(STDIN_FILENO,&sfd_set);
        FD_SET(sfd,&sfd_set);
        ret=select(sfd+1,&sfd_set,0,0,0);
        if(FD_ISSET(STDIN_FILENO,&sfd_set))
        {
            bzero(buf,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));
            if(!ret)
            {
                printf("leave\n");
                break;
            }
            send(sfd,buf,strlen(buf)-1,0);
        }
        if(FD_ISSET(sfd,&sfd_set))
        {
            bzero(buf,sizeof(buf));
            ret=recv(sfd,buf,sizeof(buf),0);
            ERROR_CHECK(ret,-1,"recv");
            if(!ret)
            {
                printf("byebye\n");
                break;
            }
            printf("%s\n",buf);
        }
    }
    // //客户端先发送数据
    // ret=send(sfd,"hello",5,0);
    // ERROR_CHECK(ret,-1,"send");
    // //客户端接收数据
    // ret=recv(sfd,buf,sizeof(buf),0);
    // printf("client gets=%s\n",buf);
    
    close(sfd);
    return 0;
}