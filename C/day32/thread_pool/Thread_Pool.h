#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__
#include "head.h"
#include "work_queue.h"

#define FILENAME "file"

typedef struct 
{
    int data_len;//火车头，存放后面buf的也就是火车车厢中存放数据的长度
    char buf[1000];//火车车厢长度
}train_t;

typedef struct {
    Queue_t Q;
    pthread_cond_t cond;//条件变量
    pthread_t *pthid;//存放线程id
    short start_flag;//线程池启动标志
}Thread_Pool_t;

//线程池初始化
int Thread_Pool_init(Thread_Pool_t *ptp,int thread_num,int capacity);
//启动线程池
int Thread_Pool_start(Thread_Pool_t *ptp,int thread_num);
//子线程流程
void* thread_handle(void* p);
//socket,bind,listen的初始化
int tcp_init(int* socket_fd,char* ip,char* port);
//文件下载协议 传送文件
int tran_file(int new_fd);
//循环接收数据，直到接收完所期待的数据量
int recv_n(int sfd,void* buf,int data_len);
//循环发送数据，直到发送完所期待的数据量
int send_n(int sfd,void* buf,int data_len);
#endif