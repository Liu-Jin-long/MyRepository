#include "process_pool.h"

int make_child(process_data_t *p_manage, int process_num)
{
    pid_t pid;
    int fds[2];
    int ret;
    for (int i = 0; i < process_num; i++)
    {
        ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fds); //创建一对连接的socket（既可读又可写）当作全双工管道的两端
        ERROR_CHECK(ret, -1, "socketpair")
        pid = fork();
        if (!pid)
        {
            
            close(fds[1]);
            child_handle(fds[0]); //一定要在child_handle中exit  return会变成僵尸进程
        }
        close(fds[0]);
        p_manage[i].pid = pid;
        p_manage[i].fd = fds[1];
        p_manage[i].busy = 0;

#ifdef DEBUG
        printf("p_manage[%d].fd=%d\n", i, p_manage[i].fd);
#endif
    }

    return 0;
}

int child_handle(int fd)
{
    int new_fd, ret;
    char exit_flag;
    char flag = 1;
    while (1)
    {
        //从父进程收到与客户端交流的socket_fd
        ret = recv_fd(fd, &new_fd, &exit_flag);
        //printf("%d进程 recv new fd=%d  %d\n", getpid(), new_fd, exit_flag);
        if (exit_flag)
        {
            printf("child process %d exit!!!\n", getpid());
            exit(0);
        }
        printf("%d get task, new_fd=%d\n", getpid(), new_fd);
        ret = transfer_file(new_fd);
        if (ret == 0)
        {
            printf("文件发送完成！\n");
        }
        else
        {
            printf("文件发送完成失败！\n");
        }

        close(new_fd);
        //通知父进程任务完成
        write(fd, &flag, sizeof(flag));
    }

    return 0;
}

//发送fd
int send_fd(int sfd, int fd, char exit_flag)
{
    struct msghdr msg;
    bzero(&msg, sizeof(msg));
    struct iovec msg_iov[1];
    msg_iov[0].iov_base = &exit_flag;
    msg_iov[0].iov_len = 4;
    msg.msg_iov = msg_iov;
    msg.msg_iovlen = 1;
    int cmsg_len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr *)malloc(cmsg_len);
    cmsg->cmsg_len = cmsg_len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(cmsg) = fd; //给变长结构体的最后一个成员赋值
    msg.msg_control = cmsg;
    msg.msg_controllen = cmsg_len;
    int ret = sendmsg(sfd, &msg, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    return 0;
}
//接收fd
int recv_fd(int sfd, int *fd, char *exit_flag)
{
    struct msghdr msg;
    bzero(&msg, sizeof(msg));
    struct iovec msg_iov[1]; //用户态信息
    // char buf[128] = {0};
    msg_iov[0].iov_base = exit_flag;
    msg_iov[0].iov_len = 4; //不读完用户信息 recvmsg不会堵塞
    msg.msg_iov = msg_iov;
    msg.msg_iovlen = 1;
    int cmsg_len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr *)malloc(cmsg_len); //内核控制信息
    cmsg->cmsg_len = cmsg_len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;

    msg.msg_control = cmsg;
    msg.msg_controllen = cmsg_len;
    int ret = recvmsg(sfd, &msg, 0);
    ERROR_CHECK(ret, -1, "recvmsg");
    *fd = *(int *)CMSG_DATA(cmsg); //得到子进程未使用的最小的fd（dup机制）
    // printf("recv_fd=%d\n", *fd);
    return 0;
}
