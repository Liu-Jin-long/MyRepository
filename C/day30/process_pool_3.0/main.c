#include "process_pool.h"


int main(int argc,char** argv)
{
    if(argc!=4)
    {
        printf("./process_pool_sever Ip Port Process_Num\n");
        return -1;
    }
    int process_num=atoi(argv[3]);
    //p_manage用来保存和管理所有子进程信息
    process_data_t *p_manage=(process_data_t*)calloc(process_num,sizeof(process_data_t));
    make_child_process(p_manage,process_num);
    int socket_fd;
    tcp_init(&socket_fd,argv[1],argv[2]);
    printf("socket_fd=%d\n",socket_fd);
    //初始化epoll，父进程既要监控socket_fd客户端请求，又要监控每个子进程的管道对端 是否完成任务
    int epfd=epoll_create(1);
    int ret=epoll_add(epfd,socket_fd);
    SELFFUNC_ERR_CHECK(ret,"epoll_add");
    int i,j;
    for(i=0;i<process_num;i++)
    {
        ret=epoll_add(epfd,p_manage[i].fd);
        SELFFUNC_ERR_CHECK(ret,"epoll_add");
    }
    //父进程监视等待客户端请求连接，一旦有客户端连接，通过accept接收并得到new_fd
    //父进程将new_fd传递给非忙碌的子进程，若收到子进程的通知，父进程将其标记为忙碌状态
    int new_fd;
    int ready_fd_num;//就绪的描述符数量
    struct epoll_event *evs=(struct epoll_event*)calloc(process_num+1,sizeof(struct epoll_event));
    struct sockaddr_in client_addr;
    
    char flag;
    while(1)
    {
        ready_fd_num=epoll_wait(epfd,evs,process_num+1,-1);//-1永久等待
        for(i=0;i<ready_fd_num;i++)
        {
            if(evs[i].data.fd==socket_fd)
            {
                bzero(&client_addr,sizeof(client_addr));
                socklen_t addr_len=sizeof(client_addr);
                new_fd=accept(socket_fd,(struct sockaddr*)&client_addr,&addr_len);
                printf("Ip=%s Port=%d has connected.\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                for(j=0;j<process_num;j++)
                {
                    if(0==p_manage[j].busy)
                    {
                        send_fd(p_manage[j].fd,new_fd);
                        p_manage[j].busy=1;
                        printf("child process %d is busy\n",p_manage[j].pid);
                        break;
                    }
                }
                close(new_fd);//无论是否找到空闲子进程，都关闭new_fd
            }
            //若收到子进程的通知，则父进程将子进程标记改为非忙碌状态
            for(j=0;j<process_num;j++)
            {
                if(evs[i].data.fd==p_manage[j].fd)
                {
                    read(p_manage[j].fd,&flag,sizeof(flag));
                    p_manage[j].busy=0;
                    printf("child process %d isn't busy\n",p_manage[j].pid);
                    break;
                }
                
            }
        }
    }
    return 0;  
}