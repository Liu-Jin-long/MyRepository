#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "head.h"
#include "work_que.h"
#define FILENAME "file"
typedef struct
{
    Que_t que;
    pthread_cond_t cond;
    pthread_t *pthid;
    short start_flag; //线程池启动标志
} Factory_t;

typedef struct
{
    int data_len; // buf中装载的实际数据长度
    char buf[1000];
} train_t;

//线程池初始化
int factory_init(Factory_t *pf, int thread_num, int capacity);
//线程池启动
int factory_start(Factory_t* pf, int thread_num);
void *thread_handle(void *p);
//初始化TCP
int tcp_init(int *socket_fd, char *ip, char *port);
//传输文件
int transfer_file(int sfd);
int send_n(int sfd, void *buf, int data_len);
int epoll_add(int epfd,int fd);
#endif