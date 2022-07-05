#include <func.h>

int main(int argc,char** argv)
{
    ARGS_CHECK(argc,3);
    int sfd=socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    printf("sfd=%d\n",sfd);
    struct sockaddr_in my_addr;//Structure describing an Internet socket address.
    bzero(&my_addr,sizeof(my_addr));//要清空
    my_addr.sin_family=AF_INET;//AF_INET代表ipv4，AF_INET6代表ipv6
    my_addr.sin_addr.s_addr=inet_addr(argv[1]);//点分十进制转为网络字节序
    my_addr.sin_port=htons(atoi(argv[2]));//端口号转为网络字节序
    
    int ret=bind(sfd,(struct sockaddr*)&my_addr,sizeof(my_addr));//绑定,结构体指针强制转换为了向老版兼容
    ERROR_CHECK(ret,-1,"bind");
    
    char buf[128]={0};
    struct sockaddr_in client_addr;//客户端的socket
    bzero(&client_addr,sizeof(client_addr));
    socklen_t addr_len=sizeof(client_addr);
    ret=recvfrom(sfd,buf,sizeof(buf),0,(struct sockaddr*)&client_addr,&addr_len);
    printf("client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    
    fd_set sfd_set;
    while (1)
    {   
        FD_ZERO(&sfd_set);
        FD_SET(STDIN_FILENO,&sfd_set);
        FD_SET(sfd,&sfd_set);
        ret=select(sfd+1,&sfd_set,NULL,NULL,NULL);
        if(FD_ISSET(STDIN_FILENO,&sfd_set))
        {
            bzero(buf,sizeof(buf));
            read(STDIN_FILENO,buf,sizeof(buf));
            sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr*)&client_addr,sizeof(client_addr));
        }
        if(FD_ISSET(sfd,&sfd_set))
        {
            bzero(buf,sizeof(buf));
            ret=recvfrom(sfd,buf,sizeof(buf)-1,0,(struct sockaddr*)&client_addr,&addr_len);
            ERROR_CHECK(ret,-1,"recvfrom");
            printf("%s\n",buf);
        }
    }
    close(sfd);
    
}