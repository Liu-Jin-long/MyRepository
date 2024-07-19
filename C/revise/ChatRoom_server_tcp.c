#include <func.h>
#define MAX_CLIENT 10
typedef struct
{
    char name[20];
    bool first;
} user;
int send_all(char *buf, user *users, int fd)
{
    for (int i = 4; i < 20; i++)
    {
        if (strlen(users[i].name) && i != fd) //自己的消息不转发给自己
        {

            int ret = send(i, buf, strlen(buf) - 1, 0);
            ERROR_CHECK(ret, -1, "send")
        }
    }
    return 0;
}
int main(int argc, char **argv)
{
    ARGS_CHECK(argc, 3)
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("sfd=%d\n", sfd); // sfd=3
    ERROR_CHECK(sfd, -1, "socket")
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    int ret = bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret, -1, "bind");
    ret = listen(sfd, MAX_CLIENT);
    ERROR_CHECK(ret, -1, "listen");
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    bzero(&client_addr, sizeof(client_addr));

    user users[20];
    bzero(users, sizeof(users));
    fd_set rdset, realset;
    FD_ZERO(&realset);
    FD_SET(STDIN_FILENO, &realset);
    FD_SET(sfd, &realset);
    char buf[128] = {0}, tmpbuf[128] = {0};
    int new_fd;
    while (1)
    {
        memcpy(&rdset, &realset, sizeof(realset));
        ret = select(10, &rdset, NULL, NULL, NULL);
        ERROR_CHECK(ret, -1, "select")
        if (FD_ISSET(sfd, &rdset))
        {
            new_fd = accept(sfd, (struct sockaddr *)&client_addr, &addr_len);
            ERROR_CHECK(new_fd, -1, "accept");
            printf("clientp ip=%s, port=%d new_fd=%d已连接\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), new_fd);
            FD_SET(new_fd, &realset);
            users[new_fd].first = true;
            strcpy(users[new_fd].name, "1");
        }
        if (FD_ISSET(STDIN_FILENO, &rdset))
        {
            bzero(buf, sizeof(buf));
            ret = read(STDIN_FILENO, buf, sizeof(buf));
            ERROR_CHECK(ret, -1, "read")
            if (!ret)
            {
                printf("我方断开。。。\n");
                sprintf(buf, "服务器：已断开\n%s", buf);
                ret = send_all(buf, users, STDIN_FILENO);
                ERROR_CHECK(ret, -1, "send_all");
                break;
            }
            else
            {
                strcpy(tmpbuf, buf);
                sprintf(buf, "服务器：%s", tmpbuf);
                ret = send_all(buf, users, STDIN_FILENO);
                ERROR_CHECK(ret, -1, "send_all");
            }
        }

        for (int i = 4; i < 20; i++)
        {
            if (strlen(users[i].name))
            {
                if (FD_ISSET(i, &rdset))
                {
                    bzero(buf, sizeof(buf));
                    ret = recv(i, buf, sizeof(buf), 0);
                    // printf("i=%d\n", i);
                    ERROR_CHECK(ret, -1, "recv")
                    if (!ret)
                    {

                        bzero(buf, sizeof(buf));
                        sprintf(buf, "%s离开聊天室。。。\n", users[i].name);
                        printf("%s", buf);
                        FD_CLR(i, &realset);
                        bzero(users + i, sizeof(user));
                        //发送给所有人buf
                        ret = send_all(buf, users, i);
                        ERROR_CHECK(ret, -1, "send_all");
                        close(i);
                    }
                    else
                    {
                        if (users[i].first)
                        {
                            strcpy(users[i].name, buf);
                            // bzero(buf, sizeof(buf));
                            sprintf(buf, "%s进入聊天室。。。\n", users[i].name);
                            printf("%s", buf);
                            ret = send_all(buf, users, i);
                            ERROR_CHECK(ret, -1, "send_all");
                            users[i].first = false;
                        }
                        else
                        {
                            bzero(tmpbuf, sizeof(tmpbuf));
                            strcat(tmpbuf, buf);
                            sprintf(buf, "%s：%s\n", users[i].name, tmpbuf);
                            printf("%s", buf);
                            ret = send_all(buf, users, i);
                            ERROR_CHECK(ret, -1, "send_all");
                        }
                    }
                }
            }
        }
    }

    close(new_fd);
    close(sfd);
    return 0;
}