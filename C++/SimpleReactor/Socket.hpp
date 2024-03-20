#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__
#include <sys/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
class Socket
{
public:
    static int socket_create()
    {
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (-1 == socket_fd)
        {
            std::cerr << "socket error!" << std::endl;
            exit(2);
        }

        int opt = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//端口重用
        return socket_fd;
    }
    static void socket_bind(int socket_fd, const char *ip, int port)
    {
        struct sockaddr_in local = {0};
        local.sin_family = AF_INET;
        local.sin_port = htons(port);
        local.sin_addr.s_addr = inet_addr(ip);
        socklen_t len = sizeof(local);
        int ret = bind(socket_fd, (struct sockaddr *)&local, len);
        if (-1 == ret)
        {
            std::cerr << "bind error!" << std::endl;
            exit(3);
        }
    }
    static void socket_listen(int socket_fd, int backlog)
    {
        int ret = listen(socket_fd, backlog);
        if (-1 == ret)
        {
            std::cerr << "listen error!" << std::endl;
            exit(4);
        }
    }
};
#endif