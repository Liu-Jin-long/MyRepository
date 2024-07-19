#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int main()
{
    char ip[]="192.168.1.1";
    struct in_addr myaddr;
    int ret=inet_aton(ip,&myaddr);
    printf("%x\n",myaddr.s_addr);
    printf("%x\n",inet_addr(ip));
    ret=inet_pton(AF_INET,ip,&myaddr);
    printf("%x\n",myaddr.s_addr);
    myaddr.s_addr=0x101a8c0;
    printf("%s\n",inet_ntoa(myaddr));
    inet_ntop(AF_INET,&myaddr,ip,16);
    puts(ip);
    return 0;
}