#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#define ARGS_CHECK(argc, num)       \
    {                               \
        if (argc != num)            \
        {                           \
            printf("error args\n"); \
            return -1;              \
        }                           \
    }
#define ERROR_CHECK(ret, retval, func_name) \
    {                                       \
        if (ret == retval)                  \
        {                                   \
            printf("errno=%d,", errno);     \
            fflush(stdout);                 \
            perror(func_name);              \
            return -1;                      \
        }                                   \
    }
#define THREAD_ERR_CHECK(ret, func_name)                                   \
    {                                                                      \
        if (ret != 0)                                                      \
        {                                                                  \
            printf("%s failed!!!,%d %s\n", func_name, ret, strerror(ret)); \
            return -1;                                                     \
        }                                                                  \
    }
#define SELFFUNC_ERR_CHECK(ret, func_name)       \
    {                                            \
        if (ret != 0)                            \
        {                                        \
            printf("%s failed!!!\n", func_name); \
            return -1;                           \
        }                                        \
    }
#define FILENAME "file"
typedef struct
{
    pid_t pid; //每个子进程pid
    int fd;    //子进程对端
    char busy; //子进程忙碌状态
} process_data_t;
typedef struct
{
    int data_len; // buf中装载的实际数据长度
    char buf[1000];
} train_t;

//创建多个子进程
int make_child(process_data_t *p_manage, int process_num);
//子进程处理函数
int child_handle(int fd);
//初始化tcp
int tcp_init(int *socket_fd, char *ip, char *port);
//添加监控描述符
int epoll_add(int epfd, int fd);
//发送fd
int send_fd(int sfd, int fd, char exit_flag);
//接收fd
int recv_fd(int sfd, int *fd, char *exit_flag);
//发送文件
int transfer_file(int sfd);
//循环接受指定的数据
int recv_n(int sfd, void *buf, int data_len);
//循环发送指定的数据
int send_n(int sfd, void *buf, int data_len);
//循环接受指定的数据并展示下载进度
int recv_n_display(int sfd, void *buf, int data_len);
