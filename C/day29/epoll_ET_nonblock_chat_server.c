#include <func.h>
//通过epoll的边沿触发，非阻塞，实现服务器和客户端聊天

//接收buf较小，若发来的信息过长，会多次水平触发读完;改为边沿触发，只会触发一次，让read非阻塞，触发一次就把接收缓冲区数据读空
//epoll对文件描述符的操作有两种模式：(默认)LT(levei trigger)和ET(edge trigger)
int fcntl_nonblock(int fd)
{
    int status=fcntl(fd,F_GETFL);//取出fd所有的属性
    status=status|O_NONBLOCK;
    int ret=fcntl(fd,F_SETFL,status);
    ERROR_CHECK(ret,-1,"fcntl");
    return 0;
}

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
    //accept自动完成三次握手，此时若无连接，accept会阻塞
    int new_sfd=accept(sfd,(struct sockaddr*)&client_addr,&addr_len);
    ERROR_CHECK(new_sfd,-1,"accept");
    printf("Connection successful. client ip=%s,port=%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
    
    char buf[6]={0};
    int epfd=epoll_create(1);//得到一个epoll的句柄(也叫描述符)
    struct epoll_event event,ready_event[2];
    event.data.fd=STDIN_FILENO;//注册标准输入
    event.events=EPOLLIN;//监控是否可读
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl1");

    fcntl_nonblock(new_sfd);//修改new_sfd为非阻塞

    event.data.fd=new_sfd;
    event.events=EPOLLIN|EPOLLET;//默认水平触发，修改为边沿触发
    ret=epoll_ctl(epfd,EPOLL_CTL_ADD,new_sfd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl2");
    int ready_fd_num,i;
    while(1)
    {
        ready_fd_num=epoll_wait(epfd,ready_event,2,-1);
        for(i=0;i<ready_fd_num;i++)
        {
            
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
                while(1)
                {
                    bzero(buf,sizeof(buf));
                    ret=recv(new_sfd,buf,sizeof(buf)-1,0);//最后一位注意要有/0
                    if(0==ret)//对方断开
                    {
                        printf("byebye\n");
                        return 0;
                    }else if(-1==ret)//非阻塞返回-1
                    {
                        printf("\n");
                        break;//读空
                    }else
                    {
                        printf("%s",buf);
                    }
                    
                }
                    
            }
        }
    }


    // //服务器先接收数据
    // ret=recv(new_sfd,buf,sizeof(buf),0);
    // ERROR_CHECK(ret,-1,"recv");
    // printf("server gets=%s\n",buf);

    // //服务器发送数据
    // ret=send(new_sfd,"world",5,0);
    // ERROR_CHECK(ret,-1,"send");

    close(sfd);
    close(new_sfd);
    return 0;
}