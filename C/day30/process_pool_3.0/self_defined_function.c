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
        recv_fd(fd,&new_fd);//接受任务
        printf("child process gets a task,new_fd=%d\n",new_fd);
        tran_file(new_fd);//发文件给客户端
        printf("begin to send data to client,ret=%d\n",ret);//子进程开始给请求客户端发送文件数据
        close(new_fd);
        write(fd,&flag,sizeof(flag));//通知父进程完成任务
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


//做一种仪式，让内核控制信息发生进程间的dup机制
int send_fd(int sfd,int fd)
{
    struct msghdr msg;
    bzero(&msg,sizeof(msg));//把结构体清空
    struct iovec iov[1];//用户态要传递一些内容
    char buf[128]="hello";
    iov[0].iov_base=buf;
    iov[0].iov_len=5;
    msg.msg_iov=iov;
    msg.msg_iovlen=1;
    //构造变长结构体
    int cmsg_len=CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg=(struct cmsghdr *)malloc(cmsg_len);
    cmsg->cmsg_len=cmsg_len;
    cmsg->cmsg_level=SOL_SOCKET;
    cmsg->cmsg_type=SCM_RIGHTS;
    *(int*)CMSG_DATA(cmsg)=fd;//给变长结构体的最后一个成员赋值
    msg.msg_control=cmsg;
    msg.msg_controllen=cmsg_len;
    int ret;
    ret=sendmsg(sfd,&msg,0);
    ERROR_CHECK(ret,-1,"sendmsg");
    return 0;
}
//接收描述符
int recv_fd(int sfd,int *fd)
{
    struct msghdr msg;
    bzero(&msg,sizeof(msg));//把结构体清空
    struct iovec iov[1];//用户态要传递一些内容
    char buf[128]={0};
    iov[0].iov_base=buf;
    iov[0].iov_len=5;
    msg.msg_iov=iov;
    msg.msg_iovlen=1;
    //构造变长结构体
    int cmsg_len=CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg=(struct cmsghdr *)malloc(cmsg_len);
    cmsg->cmsg_len=cmsg_len;
    cmsg->cmsg_level=SOL_SOCKET;
    cmsg->cmsg_type=SCM_RIGHTS;

    msg.msg_control=cmsg;
    msg.msg_controllen=cmsg_len;
    int ret;
    ret=recvmsg(sfd,&msg,0);
    ERROR_CHECK(ret,-1,"recvmsg");
    *fd=*(int*)CMSG_DATA(cmsg);
    return 0;
}

//文件下载协议 传送文件
int tran_file(int new_fd)
{
    //先发送文件名
    train_t t;
    t.data_len=strlen(FILENAME);
    strcpy(t.buf,FILENAME);
    send(new_fd,&t,4+t.data_len,0);
    //发送文件内容
    int fd=open(FILENAME,O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    while(t.data_len=read(fd,t.buf,sizeof(t.buf)))
    {
        send(new_fd,&t,4+t.data_len,0);
    }
    t.data_len=0;
    send(new_fd,&t,4+t.data_len,0);//发送文件内容完成标志
}

//循环接收数据，直到接收到所期望的数据
int recv_n(int sfd,void* buf,int data_len)
{
    char* p=(char*)buf;
    int total=0,ret;
    while(total<data_len)
    {
        ret=recv(sfd,p+total,data_len-total,0);
        total+=ret;
    }
}