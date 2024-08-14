#include "common.h"
#include "tcp_epoll_op.h"
#include "transfer_file.h"
#include "client_op.h"
int main(int argc, char **argv)
{
    char *ip, *port;
    if (argc == 3)
    {
        ip = argv[1];
        port = argv[2];
    }
    else
    {
        ip = IP;
        port = PORT;
    }
    printf("ip=%s port=%s\n", ip, port);
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(atoi(port));
    int ret;
    int option = 0;
    while (1)
    {
        // 选择注册还是登录
        printf("请选择:\n");
        printf("1、注册\n");
        printf("2、登录\n");
        scanf("%d", &option);
        if (1 == option)
        {
            printf("正在准备注册。。。\n");
            option = REGISTER;
            break;
        }
        else if (2 == option)
        {
            printf("正在准备登录。。。\n");
            option = LOGIN;
            break;
        }
        else
        {
            printf("选择错误请重新选择。。。\n");
            while (getchar() != '\n')
                ;
        }
    }
    printf("请输入用户名:\n");
    char user_name[50] = {0};
    scanf("%s", user_name);
    printf("请输入密码\n");
    char password[100] = {0};
    scanf("%s", password);
    // 发送用户名和密码
    state_train_t state_data = {0};
    state_data.state = option;
    sprintf(state_data.buf, "%s %s", user_name, password); // 以空格隔开用户名和密码
    state_data.data_len = sizeof(int) + strlen(state_data.buf);
    ret = connect(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    ERROR_CHECK(ret, -1, "connect")
    // 接收注册或登陆是否成功状态的服务器反馈
    ret = send_n(sfd, &state_data, sizeof(int) + state_data.data_len);
    SELFFUNC_ERR_CHECK(ret, "send_n")
    bzero(&state_data, sizeof(state_train_t));
    ret = recv_n(sfd, &state_data.data_len, sizeof(int));
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    ret = recv_n(sfd, &state_data.state, state_data.data_len);
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    if (option == REGISTER)
    {
        if (state_data.state == UserNameAlreadyExists)
        {
            printf("用户名已存在!!!请更换用户名进行注册!\n");
            return -1;
        }
        else if (state_data.state == RegisteredSuccessfully)
        {
            printf("%s注册成功!\n", user_name);
        }
    }
    else if (option == LOGIN)
    {
        if (state_data.state == UserNameDoesNotExist)
        {
            printf("用户名不存在!!!\n");
            return -1;
        }
        else if (state_data.state == PasswordError)
        {
            printf("密码错误!\n");
            return -1;
        }
        else if (state_data.state == LoginSuccessfully)
        {
            printf("%s登录成功!\n", user_name);
        }
    }
    // 接收Token
    bzero(&state_data, sizeof(state_train_t));
    ret = recv_n(sfd, &state_data.data_len, sizeof(int));
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    ret = recv_n(sfd, &state_data.state, state_data.data_len);
    if (state_data.state != TOKEN)
    {
        printf("TOKEN标志错误\n");
        return -1;
    }
    // printf("token=%s\n", state_data.buf);
    char token[TOKEN_LEN] = {0};
    strncpy(token, state_data.buf, TOKEN_LEN);
    SELFFUNC_ERR_CHECK(ret, "recv_n")
    int epfd = epoll_create(1);
    epoll_add(epfd, STDIN_FILENO);
    epoll_add(epfd, sfd);
    struct epoll_event events[2] = {0};
    char buf[100] = {0};
    int ready_num = 0, i = 0, j = 0, buf_len = 0;
    unsigned char has_space = 0;
    pthread_t pthid;
    p_client_child_pthread_info p_pthread_info = NULL;
    while (1)
    {
        printf("@%s # ", user_name);
        fflush(stdout); 
        ready_num = epoll_wait(epfd, events, 2, -1);
        for (i = 0; i < ready_num; ++i)
        {
            if (events[i].data.fd == STDIN_FILENO)
            {
                memset(buf, 0, sizeof(buf));
                ret = read(STDIN_FILENO, buf, sizeof(buf));
                ERROR_CHECK(ret, -1, "read")
                if (!ret)
                {
                    printf("ctrl+d断开客户端\n");
                    return 0;
                }
                buf_len = strlen(buf) - 1;
                buf[buf_len] = '\0'; // 去除最后的\n
                char cmd[10] = {0};
                has_space = 0;
                for (j = 0; j < buf_len; j++)
                {
                    if (' ' == buf[j])
                    {
                        has_space = 1;
                        break;
                    }
                    cmd[j] = buf[j];
                }
                memset(&state_data, 0, sizeof(state_train_t));
                strcpy(state_data.buf, buf);
                state_data.data_len = sizeof(int) + strlen(state_data.buf);
                if (0 == strcmp(cmd, "ls"))
                {
                    state_data.state = LS;
                    ret = send_n(sfd, &state_data, sizeof(int) + state_data.data_len);
                    SELFFUNC_ERR_CHECK(ret, "send_n")
                    memset(&state_data, 0, sizeof(state_train_t));
                    ret = recv_n(sfd, &state_data.data_len, sizeof(int));
                    SELFFUNC_ERR_CHECK(ret, "send_n")
                    ret = recv_n(sfd, &state_data.state, state_data.data_len);
                    SELFFUNC_ERR_CHECK(ret, "send_n")
                    if (state_data.state == LS)
                    {
                        printf("%s", state_data.buf);
                    }
                    else
                    {
                        printf("服务器发送LS标志错误!\n");
                    }
                }
                else if (0 == strcmp(cmd, "cd"))
                {
                    // 输入命令要有空格以及空格之后要有内容
                    if (has_space && strlen(buf) > strlen(cmd) + 1)
                    {
                        state_data.state = CD;
                        ret = send_n(sfd, &state_data, sizeof(int) + state_data.data_len);
                        SELFFUNC_ERR_CHECK(ret, "send_n")
                    }
                    else
                    {
                        printf("输入命令: %s 不符合要求!\n", buf);
                    }
                }
                else if (0 == strcmp(cmd, "rm"))
                {
                    if (has_space && strlen(buf) > strlen(cmd) + 1)
                    {
                        state_data.state = RM;
                        ret = send_n(sfd, &state_data, sizeof(int) + state_data.data_len);
                        SELFFUNC_ERR_CHECK(ret, "send_n")
                    }
                    else
                    {
                        printf("输入命令: %s 不符合要求!\n", buf);
                    }
                }
                else if (0 == strcmp(cmd, "pwd"))
                {
                    state_data.state = PWD;
                    ret = send_n(sfd, &state_data, sizeof(int) + state_data.data_len);
                    SELFFUNC_ERR_CHECK(ret, "send_n")
                    memset(&state_data, 0, sizeof(state_train_t));
                    ret = recv_n(sfd, &state_data.data_len, sizeof(int));
                    SELFFUNC_ERR_CHECK(ret, "send_n")
                    ret = recv_n(sfd, &state_data.state, state_data.data_len);
                    SELFFUNC_ERR_CHECK(ret, "send_n")
                    if (state_data.state == PWD)
                    {
                        printf("%s\n", state_data.buf);
                    }
                    else
                    {
                        printf("服务器发送PWD标志错误!\n");
                    }
                }
                else if (0 == strcmp(cmd, "mkdir"))
                {
                    // 输入命令要有空格以及空格之后要有内容
                    if (has_space && strlen(buf) > strlen(cmd) + 1)
                    {
                        state_data.state = MKDIR;
                        ret = send_n(sfd, &state_data, sizeof(int) + state_data.data_len);
                        SELFFUNC_ERR_CHECK(ret, "send_n")
                    }
                    else
                    {
                        printf("输入命令: %s 不符合要求!\n", buf);
                    }
                }
                else if (0 == strcmp(cmd, "puts") || 0 == strcmp(cmd, "gets"))
                {
                    if (has_space && strlen(buf) > strlen(cmd) + 1)
                    {
                        p_pthread_info = (p_client_child_pthread_info)malloc(sizeof(client_child_pthread_info_t));
                        memset(p_pthread_info, 0, sizeof(client_child_pthread_info_t));
                        p_pthread_info->p_server_addr = (struct sockaddr *)&server_addr;
                        p_pthread_info->state = 0 == strcmp(cmd, "puts") ? PUTS : GETS;
                        strncpy(p_pthread_info->token, token, TOKEN_LEN);
                        strcpy(p_pthread_info->buf, state_data.buf);
                        ret = pthread_create(&pthid, NULL, child_pthread_func, p_pthread_info);
                        THREAD_ERR_CHECK(ret, "pthread_create")
                    }
                    else
                    {
                        printf("输入命令: %s 不符合要求!\n", buf);
                    }
                }
                else
                {
                    printf("不支持的命令%s\n", cmd);
                }
            }

            if (events[i].data.fd == sfd)
            {
                char exit_sign[1] = {0};
                ret = recv_n(sfd, exit_sign, sizeof(exit_sign));
                if (ret == -1)
                {
                    printf("服务器关闭连接\n");
                    return 0;
                }
            }
        }
    }
}