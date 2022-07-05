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
    
    int ret=bind(sfd,(struct sockaddr*)&my_addr,sizeof(my_addr));//绑定,结构体指针强制转换为了向老版兼容
    ERROR_CHECK(ret,-1,"bind");
    //开始监听，端口开启
    ret=listen(sfd,10);//“同时”能处理的最大的连接请求
    ERROR_CHECK(ret,-1,"listen");
    
    struct sockaddr_in client_addr;//new_sfd才是和客户端进行交流的描述符
    bzero(&client_addr,sizeof(client_addr));
    socklen_t addr_len=sizeof(client_addr);
    //accept自动完成三次握手，此时若无连接，accept会阻塞
    int new_sfd=accept(sfd,(struct sockaddr*)&client_addr,&addr_len);
    ERROR_CHECK(new_sfd,-1,"accept");
    printf("Connection successful. client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    
    char buf[128]={0};
    fd_set sfd_set,temp_set;
    FD_ZERO(&sfd_set);
    FD_SET(STDIN_FILENO,&sfd_set);
    FD_SET(sfd,&sfd_set);
    while(1)
    {
        //清空集合，写入要监视的网络套接字描述符
        FD_ZERO(&sfd_set);
        FD_SET(STDIN_FILENO,&sfd_set);
        FD_SET(new_sfd,&sfd_set);
        ret=select(new_sfd+1,&sfd_set,0,0,0);
        if(FD_ISSET(STDIN_FILENO,&sfd_set))
        {
            bzero(buf,sizeof(buf));
            ret=read(STDIN_FILENO,buf,sizeof(buf));
            if(!ret)
            {
                printf("leave\n");
                break;
            }
            send(new_sfd,buf,strlen(buf)-1,0);
        }
        if(FD_ISSET(new_sfd,&sfd_set))
        {
            bzero(buf,sizeof(buf));
            ret=recv(new_sfd,buf,sizeof(buf),0);
            ERROR_CHECK(ret,-1,"recv");
             if(!ret)
            {
                printf("byebye\n");
                break;
            }
            printf("%s\n",buf);
        }
    }


    // //服务器先接收数据
    // ret=recv(new_sfd,buf,sizeof(buf),0);
    // ERROR_CHECK(ret,-1,"recv");
    // printf("server gets=%s\n",buf);

    // //服务器发送数据
    // ret=send(new_sfd,"world",5,0);
    // ERROR_CHECK(ret,-1,"send");

    close(sfd);
    close(new_sfd);
    return 0;
}