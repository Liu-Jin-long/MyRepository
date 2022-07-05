#include "process_pool.h"

int make_child_process(process_data_t* p_manage,int process_num)
{
    int i,ret;
    pid_t pid;
    int fds[2];//父进程和子进程之间的管道
    for(int i=0;i<process_num;i++)
    {
        ret=socketpair(AF_LOCAL,SOCK_STREAM,0,fds);
        ERROR_CHECK(ret,-1,"socketpair");
        pid=fork();
        if(0==pid)
        {//子进程
            close(fds[1]);
            child_handle(fds[0]);//一定要让子进程child_handle内exit
        }
        close(fds[0]);
        //父进程通过fds[1]和子进程通信，区别于pipe的是无需两条管道，父子进程的fds[0]和fds[1]既可读又可写
        p_manage[i].pid=pid;
        p_manage[i].fd=fds[1];
        p_manage[i].busy=0;//初始默认子进程为非忙碌状态
#ifdef DEBUG//调试开关
        printf("p_manage[%d].fd=%d\n",i,p_manage[i].fd);
#endif
    }
}


int child_handle(int fd)
{
    int new_fd,ret;//子进程从父进程接收一个new_fd,通过这个new_fd给客户端发文件
    char flag=1;
    while(1)
    {
        read(fd,&new_fd,sizeof(new_fd));//接受任务
        printf("child process gets a task,new_fd=%d\n",new_fd);
        ret=send(new_fd,"hello",5,0);
        printf("begin to send data to client,ret=%d\n",ret);//子进程开始给请求客户端发送文件数据
        write(fd,&flag,sizeof(flag));
    }
    return 0;
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

int epoll_add(int epfd,int fd)
{
    struct epoll_event event;
    event.events=EPOLLIN;
    event.data.fd=fd;
    int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}