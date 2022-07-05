#include "func.h"
//通过select实现tcp的即时聊天
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

    //客户端连接服务器
    int ret=connect(sfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
    ERROR_CHECK(ret,-1,"connect");
    
    //接受文件名
    int data_len;
    char buf[1000]={0};
    recv(sfd,&data_len,4,0);//接收文件名长度
    recv(sfd,buf,data_len,0);//接收文件名
    
    int fd=open(buf,O_WRONLY|O_CREAT,0666);//buf中存着文件名
    ERROR_CHECK(fd,-1,"open");
    while (1)
    {
        recv(sfd,&data_len,4,0);//先接文件内容长度
        if(data_len>0)
        {
            recv(sfd,buf,data_len,0);//接文件内容
            write(fd,buf,data_len);//把文件内容写到文件里
        }else{
            break;
        }
        
    }
    
    close(sfd);
    return 0;
}