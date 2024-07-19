#include <func.h>

int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 3)
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("sfd=%d\n", sfd); // sfd=3
    ERROR_CHECK(sfd, -1, "socket")
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));
    int new_fd = accept(sfd, (struct sockaddr *)&client_addr, &addr_len);
    ERROR_CHECK(new_fd, -1, "accept");
    int epoll_fd=epoll_create(1);
    struct epoll_event event,evs[2];
    event.data.fd=STDIN_FILENO;
    event.events=EPOLLIN;

    ret=epoll_ctl(epoll_fd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    event.data.fd=new_fd;
    event.events=EPOLLIN;
    ret=epoll_ctl(epoll_fd,EPOLL_CTL_ADD,new_fd,&event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    int ready_num;
    char buf[128];
    while(1)
    {
        ready_num=epoll_wait(epoll_fd,evs,2,-1);
        for(int i=0;i<ready_num;i++)
        {
            if(evs[i].data.fd==STDIN_FILENO)
            {
                bzero(buf, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read")
                if (!ret)
                {
                    printf("I want go!!!\n");
                    return 0;
                }
                ret=send(new_fd,buf,strlen(buf)-1,0);
                printf("send_ret=%d\n",ret);
                ERROR_CHECK(ret, -1, "send")
            }
            if(evs[i].data.fd==new_fd)
            {
                bzero(buf, sizeof(buf));
                ret = read(new_fd, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read")
                sleep(1);
                if (!ret)
                {
                    printf("对方断开\n");
                    return 0;
                }
                
                printf("%s\n",buf);
            }

        }
    }
    close(new_fd);
    close(sfd);
    close(epoll_fd);
    return 0;

}