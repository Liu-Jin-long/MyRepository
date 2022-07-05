#include <func.h>
//通过epoll的水平触发，实现客户端断开后，能够再次连接的即时聊天

int main(int argc,char** argv)
{
    ARGS_CHECK(argc,3);
    int sfd;
    sfd=socket(AF_INET,SOCK_STREAM,0);//套接口描述符，对应的是一个缓冲区
    ERROR_CHECK(sfd,-1,"socket");
    printf("sfd=%d\n",sfd);
    struct sockaddr_in my_addr;//Structure describing an Internet socket address.
    bzero(&my_addr,sizeof(my_addr));//要清空
    my_addr.sin_family=AF_INET;//AF_INET代表ipv4，AF_INET6代表ipv6
    my_addr.sin_addr.s_addr=inet_addr(argv[1]);//点分十进制转为网络字节序
    my_addr.sin_port=htons(atoi(argv[2]));//端口号转为网络字节序
    
    int ret=bind(sfd,(struct sockaddr*)&my_addr,sizeof(my_addr));//绑定,结构体指针强制转换为了向老版兼容
    ERROR_CHECK(ret,-1,"bind");
    //开始监听，端口开启
    ret=listen(sfd,10);//“同时”能处理的最大的连接请求
    ERROR_CHECK(ret,-1,"listen");
    
    struct sockaddr_in client_addr;//new_sfd才是和客户端进行交流的描述符
    bzero(&client_addr,sizeof(client_addr));
    socklen_t addr_len=sizeof(client_addr);
    
    
    char buf[128]={0};
    int epfd=epoll_create(1);//得到一个epoll的句柄(也叫描述符)
    struct epoll_event event,ready_event[3];

    event.data.fd=STDIN_FILENO;
    event.events=EPOLLIN;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl1");

    event.data.fd=sfd;
    event.events=EPOLLIN;
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl1");

    int ready_fd_num,i,new_sfd;
    while(1)
    {
        ready_fd_num=epoll_wait(epfd,ready_event,2,-1);
        for(i=0;i<ready_fd_num;i++)
        {
            if(ready_event->data.fd==sfd)
            {
                //accept自动完成三次握手，此时若无连接，accept会阻塞
                new_sfd=accept(sfd,(struct sockaddr*)&client_addr,&addr_len);
                ERROR_CHECK(new_sfd,-1,"accept");
                printf("Connection successful. client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
                event.data.fd=new_sfd;
                event.events=EPOLLIN;
                ret=epoll_ctl(epfd,EPOLL_CTL_ADD,new_sfd,&event);
                ERROR_CHECK(ret,-1,"epoll_ctl2");
            }
            
            if(ready_event[i].data.fd==STDIN_FILENO)
            {
                bzero(buf,sizeof(buf));
                ret=read(STDIN_FILENO,buf,sizeof(buf));
                if(!ret)
                {
                    printf("leave\n");
                    return 0;
                }
                send(new_sfd,buf,strlen(buf)-1,0);
            }
            if(ready_event[i].data.fd==new_sfd)
            {
                bzero(buf,sizeof(buf));
                ret=recv(new_sfd,buf,sizeof(buf),0);
                if(0==ret)//对方断开
                {
                    printf("byebye\n");
                    event.data.fd=new_sfd;
                    event.events=EPOLLIN;
                    ret=epoll_ctl(epfd,EPOLL_CTL_DEL,new_sfd,&event);
                    ERROR_CHECK(ret,-1,"epoll_ctl2");
                    
                    close(new_sfd);
                    break;
                }
                printf("%s\n",buf);
                    
            }
        }
    }

    close(sfd);
    close(new_sfd);
    return 0;
}