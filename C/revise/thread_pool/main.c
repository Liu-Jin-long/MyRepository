#include "factory.h"
int exit_fd[2];
void sig_exit_func(int signum)
{
    write(exit_fd[1], &signum, 1);
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        printf("./thread_pool_server IP PORT THREAD_NUM CAPACITY\n");
    }

    pipe(exit_fd);
    if (fork()) //父进程协助完成子进程线程池有序退出机制
    {
        close(exit_fd[0]);
        signal(SIGQUIT, sig_exit_func);//不要用ctrl+c，用kill给父进程信号
        wait(NULL);
        return 0;
    }
    Factory_t f;
    int thread_num = atoi(argv[3]);
    int capacity = atoi(argv[4]);
    factory_init(&f, thread_num, capacity);
    factory_start(&f, thread_num);
    int socket_fd, new_fd;
    tcp_init(&socket_fd, argv[1], argv[2]);
    struct sockaddr_in client_addr;
    pNode_t pnew;
    pQue_t pq = &f.que;
    struct epoll_event evs[2];
    int epfd = epoll_create(1);
    epoll_add(epfd, socket_fd);

    epoll_add(epfd, exit_fd[0]);
    int ready_num, i, j;

    while (1)
    {
        ready_num = epoll_wait(epfd, evs, 2, -1);
        for (i = 0; i < ready_num; i++)
        {
            if (evs[i].data.fd == socket_fd)
            {
                socklen_t addr_len = sizeof(client_addr);
                bzero(&client_addr, sizeof(client_addr));
                new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_len);
                ERROR_CHECK(new_fd, -1, "accept")
                printf("%s %d is connected!\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                pnew = (pNode_t)malloc(sizeof(Node_t));
                pnew->new_fd = new_fd;
                pthread_mutex_lock(&pq->mutex);
                que_set(pq, pnew);
                pthread_mutex_unlock(&pq->mutex);
                pthread_cond_signal(&f.cond);
            }
            if (evs[i].data.fd == exit_fd[0]) //线程池退出
            {
                for (j = 0; j < thread_num; j++)
                {
                    pthread_cancel(f.pthid[j]);
                    printf("cancel sucess!\n");
                }
                for (j = 0; j < thread_num; j++)
                {
                    pthread_join(f.pthid[j],NULL);
                }
                exit(0);
            }
        }
    }
    return 0;
}