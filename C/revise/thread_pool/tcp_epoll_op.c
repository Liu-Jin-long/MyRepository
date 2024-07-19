#include "factory.h"
int tcp_init(int *socket_fd,char* ip,char* port)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket")
    int ret,resue=1;
    ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&resue,sizeof(int));
    ERROR_CHECK(sfd, -1, "setsockopt")
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(port));
    server_addr.sin_addr.s_addr = inet_addr(ip);
    ret = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");
    *socket_fd=sfd;
    return 0;
}   
int epoll_add(int epfd,int fd)
{
    struct epoll_event event;
    event.data.fd=fd;
    event.events=EPOLLIN;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret, -1, "epoll_ctl")
    return 0;
}