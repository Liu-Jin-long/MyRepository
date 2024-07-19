#include "func.h"
void func(int signum)
{
    printf("%d is coming!\n", signum);
}
#define SA_RESTART 0x10000000
int epoll_add(int epfd, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl")
    return 0;
}
int main()
{
    struct sigaction st;
    st.sa_handler = func;
    st.sa_flags = SA_SIGINFO;
    st.sa_flags = SA_RESTART|SA_SIGINFO;
    //sigaction(SIGINT, &st, NULL);
    char buf[50] = {0};
    struct epoll_event *evs = (struct epoll_event *)calloc(+2, sizeof(struct epoll_event));
    int epfd = epoll_create(1);
    epoll_add(epfd, 0);
    epoll_add(epfd, 1);
    int ready_num;
    while (1)
    {
        // fflush(stdout);
        // fflush(stdin);
        ready_num = epoll_wait(epfd, evs, 2, -1); //收到信号epoll_wait返回-1
        printf("readynum=%d\n", ready_num);
        for (int i = 0; i < ready_num; i++)
        {
            if (evs[i].data.fd == 0)
            {
                sleep(1);
                printf("in");
            }
            if (evs[i].data.fd == 1)
            {
                sleep(1);
                printf("out");
                
            }
        }
    }
    return 0;
}