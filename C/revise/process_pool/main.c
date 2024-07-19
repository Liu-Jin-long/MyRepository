#include "process_pool.h"

int exit_pipefd[2];
//退出机制
void sig_exit_func(int signum)
{
    //随便写入内容进入管道造成读端可读（epoll_wait负责监控）
    write(exit_pipefd[1], &signum, 1); //异步拉起同步
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("./process_pool.c IP PORT PROCESS_NUM\n");
        return -1;
    }
    int process_num = atoi(argv[3]);
    process_data_t *p_manage = (process_data_t *)calloc(process_num, sizeof(process_data_t));
    int ret = make_child(p_manage, process_num);
    ERROR_CHECK(ret, -1, "make_child");
    int socket_fd, i = 0, j;
    ret = tcp_init(&socket_fd, argv[1], argv[2]);
    SELFFUNC_ERR_CHECK(ret, "tcp_init")
    int epfd = epoll_create(1);
    ret = epoll_add(epfd, socket_fd);
    for (i = 0; i < process_num; i++)
    {
        ret = epoll_add(epfd, p_manage[i].fd);
        SELFFUNC_ERR_CHECK(ret, "epoll_add")
    }

    // exit_pipefd管道传递退出信息初始化
    pipe(exit_pipefd);
    signal(SIGINT, sig_exit_func);
    ret = epoll_add(epfd, exit_pipefd[0]);
    SELFFUNC_ERR_CHECK(ret, "epoll_add")

    int new_fd, ready_num;
    struct epoll_event *evs = (struct epoll_event *)calloc(process_num + 2, sizeof(struct epoll_event));
    struct sockaddr_in client_addr;
    char flag;
    while (1)
    {
        ready_num = epoll_wait(epfd, evs, process_num + 2, -1); //收到信号epoll_wait返回-1
        for (i = 0; i < ready_num; i++)
        {
            if (evs[i].data.fd == socket_fd)
            {
                socklen_t addr_len = sizeof(client_addr);
                bzero(&client_addr, sizeof(client_addr));
                new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_len);
                ERROR_CHECK(new_fd, -1, "accept")
                printf("%s %d is connected!\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                for (j = 0; j < process_num; j++)
                {
                    if (!p_manage[j].busy)
                    {
                        ret = send_fd(p_manage[j].fd, new_fd, 0);
                        SELFFUNC_ERR_CHECK(ret, "send_fd")
                        p_manage[j].busy = 1;
                        printf("child process %d is busy!\n", p_manage[j].pid);
                        break;
                    }
                }
                close(new_fd);
            }
            //子进程通知任务完成，标记为非忙碌
            for (j = 0; j < process_num; j++)
            {
                if (evs[i].data.fd == p_manage[j].fd)
                {
                    ret = read(p_manage[j].fd, &flag, sizeof(flag));
                    ERROR_CHECK(ret, -1, "read")
                    p_manage[j].busy = 0;
                    printf("child process %d is NOT busy!\n", p_manage[j].pid);
                    break;
                }
            }
            if (evs[i].data.fd == exit_pipefd[0]) //收到退出信号
            {
                for (j = 0; j < process_num; j++)
                {

                    send_fd(p_manage[j].fd, 0, 1); //发送exit通知给子进程
                }
                for (j = 0; j < process_num; j++)
                {
                    wait(NULL); //等待回收每个子进程
                }
                close(socket_fd);
                return 0;
            }
        }
    }

    return 0;
}
