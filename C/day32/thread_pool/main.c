#include "Thread_Pool.h"

int main(int argc,char** argv)
{
    if(argc!=5)
    {
        printf("./thread_pool_server Ip Port Thread_Num Capacity");
        return -1;
    }
    int thread_num=atoi(argv[3]);
    int capacity=atoi(argv[4]);
    Thread_Pool_t tp;
    Thread_Pool_init(&tp,thread_num,capacity);
    Thread_Pool_start(&tp,thread_num);
    int socket_fd;
    tcp_init(&socket_fd,argv[1],argv[2]);
    //接收客户端请求，加锁，放入队列，解锁，signal
    int new_fd;
    struct sockaddr_in client_addr;
    pNode_t pnew;
    pQueue_t pq=&tp.Q;
    while(1)
    {
        bzero(&client_addr,sizeof(client_addr));
        socklen_t addr_len=sizeof(client_addr);
        new_fd=accept(socket_fd,(struct sockaddr*)&client_addr,&addr_len);
        printf("Ip=%s Port=%d has connected.\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        pnew=(Node_t*)calloc(1,sizeof(Node_t));//较耗时，放在锁外
        pnew->new_fd=new_fd;
        pthread_mutex_lock(&pq->mutex);//加锁，放入队列，解锁
        queue_set(pq,pnew);
        pthread_mutex_unlock(&pq->mutex);
        
        pthread_cond_signal(&tp.cond);
    }
    
    return 0;
}