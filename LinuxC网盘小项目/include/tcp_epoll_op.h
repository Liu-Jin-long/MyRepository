#ifndef __TCP_EPOLL_H__
#define __TCP_EPOLL_H__
int tcp_init(int *socket_fd,char* ip,char* port);
int epoll_add(int epfd,int fd);
int epoll_delete(int epfd,int fd);
#endif