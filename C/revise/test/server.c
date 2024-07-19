#define _XOPEN_SOURCE
#include "func.h"
int main(int argc, char** argv)
{
    ARGS_CHECK(argc, 3);
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr, client_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_family = AF_INET;
    int ret = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");
    bzero(&client_addr, sizeof(client_addr));
    socklen_t addr_len=sizeof(client_addr);
    int client_sfd = accept(sfd, (struct sockaddr *)&client_addr, &addr_len);
    ERROR_CHECK(client_sfd, -1, "accept")
    char buf[5];
    int count=0;
    while (1)
    {
        bzero(buf, sizeof(buf));
        count++;
        ret = recv(client_sfd, buf, sizeof(buf), 0);
        if (ret == -1)
        {
            printf("recv返回-1\n");
        }
        else if (ret == 0)
        {
            printf("recv返回0\n");
        }
        else{
            printf("第%d次recv=%s\n",count, buf);}
    }
    return 0;
}