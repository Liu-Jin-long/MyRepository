#define _XOPEN_SOURCE
#include "func.h"
void sig_handle(int signo)
{
    printf("%d is coming\n",signo);
}
int main(int argc, char** argv)
{
    signal(SIGPIPE,sig_handle);
    ARGS_CHECK(argc, 3);
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_family = AF_INET;
    int ret=connect(sfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    ERROR_CHECK(ret, -1, "connect")
    // bzero(&client_addr, sizeof(client_addr));
    // int client_sfd = accept(sfd, (struct sockaddr *)&client_addr, sizeof(client_addr));
    int count=0;
    char buf[65535]={0};
    // while (1)
    // {
    //     count++;
    //     strcpy(buf,"hello world");
    //     //bzero(buf, sizeof(buf));
    //     ret = send(sfd, buf, sizeof(buf), 0);
    //     if (ret == -1)
    //     {
    //         printf("send返回-1\n");
    //     }
    //     else if (ret == 0)
    //     {
    //         printf("send返回0\n");
    //     }
    //     else{
    //         printf("第%d次send=%s\n", count,buf);
    //         }
    //     //sleep(1);
    // }
    while(1);
    return 0;
}