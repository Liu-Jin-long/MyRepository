#include "Thread_Pool.h"

int Thread_Pool_init(Thread_Pool_t *ptp,int thread_num,int capacity)
{
    queue_init(&ptp->Q,capacity);
    int ret=pthread_cond_init(&ptp->cond,NULL);
    THREAD_ERR_CHECK(ret,"pthread_cond_init");
    ptp->pthid=(pthread_t*)calloc(thread_num,sizeof(pthread_t));
    ptp->start_flag=0;
    return 0;
}

int Thread_Pool_start(Thread_Pool_t *ptp,int thread_num)
{
    if(0==ptp->start_flag)
    {
        for(int i=0;i<thread_num;i++)
        {
            pthread_create(ptp->pthid+i,NULL,thread_handle,ptp);//传参，方便子线程访问
        }
        ptp->start_flag=1;
        return 0;
    }else{
        printf("Thread_Pool_start failed\n");
        return -1;
    }
}

//子线程流程
void* thread_handle(void* q)
{
    Thread_Pool_t* ptp=(Thread_Pool_t*)q;
    pQueue_t pq=&ptp->Q;
    pNode_t p=NULL;//从队列中取出队头结点
    while(1)
    {
        pthread_mutex_lock(&pq->mutex);
        if(0==pq->que_size)
        {
            pthread_cond_wait(&ptp->cond,&pq->mutex);
        }
        queue_get(pq,&p);
        pthread_mutex_unlock(&pq->mutex);
        if(p)//p不为NULL说明拿到了结点
        {
            tran_file(p->new_fd);
            free(p);
            p=NULL;
        }
    }
}

int tcp_init(int* socket_fd,char* ip,char* port)
{
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);//套接口描述符，对应的是一个缓冲区
    ERROR_CHECK(sfd,-1,"socket");
    //printf("sfd=%d\n",sfd);
    //在bind之前去执行setsockopt,设定端口重用
    int reuse=1,ret;
    ret=setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
    ERROR_CHECK(ret,-1,"setsockopt");
    
    struct sockaddr_in my_addr;//Structure describing an Internet socket address.
    bzero(&my_addr,sizeof(my_addr));//要清空
    my_addr.sin_family=AF_INET;//AF_INET代表ipv4，AF_INET6代表ipv6
    my_addr.sin_addr.s_addr=inet_addr(ip);//点分十进制转为网络字节序
    my_addr.sin_port=htons(atoi(port));//端口号转为网络字节序
    
    ret=bind(sfd,(struct sockaddr*)&my_addr,sizeof(my_addr));//绑定,结构体指针强制转换为了向老版兼容
    ERROR_CHECK(ret,-1,"bind");
    //开始监听，端口开启
    ret=listen(sfd,10);//“同时”能处理的最大的连接请求
    ERROR_CHECK(ret,-1,"listen");
    *socket_fd=sfd;//socket_fd传出参数
    return 0;
}

//文件下载协议 传送文件
int tran_file(int new_fd)
{
    //先发送文件名
    train_t t;
    t.data_len=strlen(FILENAME);
    strcpy(t.buf,FILENAME);
    send_n(new_fd,&t,4+t.data_len);
    //发送文件大小给客户端
    struct stat buf;//文件大小作为数据
    int ret=stat(FILENAME,&buf);
    t.data_len=sizeof(buf.st_size);//文件大小数据 所占长度
    memcpy(t.buf,&buf.st_size,sizeof(buf.st_size));
    send_n(new_fd,&t,4+t.data_len);
    //发送文件内容
    int fd=open(FILENAME,O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    while(t.data_len=read(fd,t.buf,sizeof(t.buf)))
    {
        send_n(new_fd,&t,4+t.data_len);
    }
    t.data_len=0;
    send_n(new_fd,&t,4+t.data_len);//发送文件内容完成标志
    close(new_fd);

}

//循环接收数据，直到接收完所期待的数据量
int recv_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        total+=ret;
    }
    return 0;
}

//循环发送数据，直到发送完所期待的数据量
int send_n(int sfd,void* buf,int data_len)
{
    char *p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=send(sfd,p+total,data_len-total,0);
        total+=ret;
    }
    return 0;
}