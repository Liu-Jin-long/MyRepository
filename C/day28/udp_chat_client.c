#include <func.h>

int main(int argc,char** argv)
{
    ARGS_CHECK(argc,3);
    int sfd=socket(AF_INET,SOCK_DGRAM,0);
    ERROR_CHECK(sfd,-1,"socket");
    printf("sfd=%d\n",sfd);
    struct sockaddr_in server_addr;//服务器端的socket
    bzero(&server_addr,sizeof(server_addr));//要清空
    server_addr.sin_family=AF_INET;//AF_INET代表ipv4，AF_INET6代表ipv6
    server_addr.sin_addr.s_addr=inet_addr(argv[1]);//点分十进制转为网络字节序
    server_addr.sin_port=htons(atoi(argv[2]));//端口号转为网络字节序

    sendto(sfd,"hello",5,0,(struct sockaddr*)&server_addr,sizeof(server_addr));
    char buf[128]={0};
    fd_set sfd_set;
    int ret;
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
            sendto(sfd,buf,strlen(buf)-1,0,(struct sockaddr*)&server_addr,sizeof(server_addr));
        }
        if(FD_ISSET(sfd,&sfd_set))
        {
            bzero(buf,sizeof(buf));
            ret=recvfrom(sfd,buf,sizeof(buf)-1,0,NULL,NULL);
            ERROR_CHECK(ret,-1,"recvfrom");
            printf("%s\n",buf);
        }
    }
    close(sfd);
    return 0;
}