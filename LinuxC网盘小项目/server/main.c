#include "common.h"
#include "thread_pool.h"
#include "operation.h"
#include "link_list.h"
#include "md5sum.h"
#include "tcp_epoll_op.h"
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
    // 如果没有文件池所在文件夹就创建
    if (-1 == access(FILE_POOL_PATH, F_OK))
    {
        if (-1 == mkdir(FILE_POOL_PATH, 0755))
        {
            printf("文件夹%s路径创建失败! 运行该项目请先确保%s文件存在!", FILE_POOL_PATH, FILE_POOL_PATH);
            return -1;
        }
    }
    int thread_num = THREAD_NUM;
    int thread_pool_capacity = THREAD_POOL_CAPACITY;
    thread_pool_t tp;
    thread_pool_init(&tp, thread_num, thread_pool_capacity);
    thread_pool_start(&tp, thread_num);
    pNode_t pnew;
    pQueue_t pq = &tp.queue;
    LinkList clientInfo_linkList = NULL;
    LinkList_Timeout roulette[TIMEOUT_DISCONNECT_SECOND] = {0};
    int socket_fd, new_fd;
    tcp_init(&socket_fd, ip, port);
    struct sockaddr_in client_addr;
    struct epoll_event events[MAX_EVENTS] = {0};
    int epfd = epoll_create(1);
    epoll_add(epfd, socket_fd);
    int maxevents = 1;
    int ready_num, i = 0, j = 0, ret = 0;
    char username[64] = {0};
    LNode *p = NULL;
    time_t past = time(NULL);
    while (1)
    {
        ready_num = epoll_wait(epfd, events, maxevents, 1000);
        if (ready_num > 0)
        {
            for (i = 0; i < ready_num; i++)
            {
                if (events[i].data.fd == socket_fd)
                {
                    printf("有新连接\n");
                    bzero(&client_addr, sizeof(client_addr));
                    bzero(username, sizeof(username));
                    socklen_t addr_len = sizeof(client_addr);
                    new_fd = accept(socket_fd, (struct sockaddr *)&client_addr, &addr_len);
                    ERROR_CHECK(new_fd, -1, "accept")
                    printf("%s %d is connected!\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    // 连接成功后接收一次信息  处理REGISTER、LOGIN或者TOKEN
                    state_train_t state_data = {0};
                    ret = recv_n(new_fd, &state_data.data_len, sizeof(int));
                    SELFFUNC_ERR_CHECK(ret, "recv_n")
                    ret = recv_n(new_fd, &state_data.state, state_data.data_len);
                    SELFFUNC_ERR_CHECK(ret, "recv_n")
                    if (state_data.state == REGISTER || state_data.state == LOGIN)
                    {
                        if (state_data.state == REGISTER)
                        {
                            // 注册
                            ret = do_register(&state_data, new_fd, username);
                        }
                        else if (state_data.state == LOGIN)
                        {
                            // 登录
                            ret = do_login(&state_data, new_fd, username);
                        }
                        if (-1 == ret)
                        {
                            // 注册或登录失败
                            printf("socket_fd=%d %s失败\n", new_fd, state_data.state == LOGIN ? "登录" : "注册");
                            close(new_fd);
                        }
                        else
                        {
                            // 注册或登录成功
                            // 先发送token
                            state_train_t token_data = {0};
                            token_data.state = TOKEN;
                            token_data.data_len = TOKEN_LEN + sizeof(int);
                            generate_random_string(token_data.buf, TOKEN_LEN);
                            ret = send_n(new_fd, &token_data, token_data.data_len + sizeof(int));
                            SELFFUNC_ERR_CHECK(ret, "send_n")
                            // 添加维护客户端信息到链表
                            client_info_t client_info = {0};
                            client_info.sfd = new_fd;
                            strcpy(client_info.username, username);
                            // 客户端登录或注册初始
                            strcpy(client_info.path, "/");
                            client_info.code = 0;
                            strncpy(client_info.token, token_data.buf, TOKEN_LEN);
                            printf("用户%s%s成功 token=%s\n", username, state_data.state == LOGIN ? "登录" : "注册", token_data.buf);
                            link_list_head_insert(&clientInfo_linkList, client_info);
                            // 加入epoll_wait监控
                            epoll_add(epfd, new_fd);
                            maxevents++;
                            // 添加到轮盘当前索引链表中
                            roulette_update_and_check(roulette, &clientInfo_linkList, &j, &past, -1, new_fd);
                        }
                    }
                    else if (state_data.state == TOKEN)
                    {
                        printf("开始Token认证\n");
                        // Token认证
                        int state = TokenAuthenticationFailed;
                        LNode *p = clientInfo_linkList;
                        while (p)
                        {
                            if (0 == strncmp(p->client_info.token, state_data.buf, TOKEN_LEN))
                            {
                                state = TokenAuthenticationSucceeded;
                                break;
                            }
                            p = p->next;
                        }
                        bzero(&state_data, sizeof(state_data));
                        if (state == TokenAuthenticationFailed)
                        {
                            printf("Token认证失败\n");
                            // Token认证失败
                            state_data.state = TokenAuthenticationFailed;
                            state_data.data_len = sizeof(state_data.state);
                            send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
                            SELFFUNC_ERR_CHECK(ret, "send_n")
                            close(new_fd);
                        }
                        else if (state == TokenAuthenticationSucceeded)
                        {
                            // Token认证成功
                            printf("Token认证成功\n");
                            state_data.state = TokenAuthenticationSucceeded;
                            state_data.data_len = sizeof(state_data.state);
                            send_n(new_fd, &state_data, state_data.data_len + sizeof(int));
                            SELFFUNC_ERR_CHECK(ret, "send_n")
                            // 接收信息 处理GETS或者PUTS
                            bzero(&state_data, sizeof(state_data));
                            ret = recv_n(new_fd, &state_data.data_len, sizeof(int));
                            SELFFUNC_ERR_CHECK(ret, "recv_n")
                            ret = recv_n(new_fd, &state_data.state, state_data.data_len);
                            SELFFUNC_ERR_CHECK(ret, "recv_n")
                            if (state_data.state == PUTS || state_data.state == GETS)
                            {
                                // 添加队列任务唤醒子线程
                                pnew = (pNode_t)calloc(1, sizeof(Node_t));
                                pnew->sfd = new_fd;
                                pnew->p_client_info = &p->client_info;
                                pnew->state = state_data.state;
                                strcpy(pnew->buf, state_data.buf);
                                pthread_mutex_lock(&pq->mutex);
                                queue_set(pq, pnew);
                                pthread_mutex_unlock(&pq->mutex);
                                pthread_cond_signal(&tp.cond);
                            }
                            else
                            {
                                // 客户端子进程的发送不是GETS或者PUTS
                                close(new_fd);
                            }
                        }
                    }
                    else
                    {
                        // 客户端新连接第一次发送的不是REGISTER、LOGIN或者TOKEN
                        close(new_fd);
                    }
                }
                else
                {
                    // 监控所有客户端命令请求
                    p = clientInfo_linkList;
                    state_train_t state_data = {0};
                    while (p)
                    {
                        // 遍历维护的所有客户端信息
                        if (events[i].data.fd == p->client_info.sfd)
                        {
                            // 先接收一次命令信息
                            ret = recv_n(p->client_info.sfd, &state_data.data_len, sizeof(int));
                            if (ret == -1)
                            {
                                printf("socket_fd=%d客户端断开连接\n", p->client_info.sfd);
                                // 删除轮盘中存在的该sfd
                                roulette_link_list_delete_node(roulette, p->client_info.sfd);
                                // 取消监控
                                epoll_delete(epfd, p->client_info.sfd);
                                //printf("epoll_delete %d\n", p->client_info.sfd);
                                close(p->client_info.sfd);
                                // 删除客户端信息结点
                                link_list_delete_node(&clientInfo_linkList, p->client_info.sfd);
                            }
                            else
                            {
                                ret = recv_n(p->client_info.sfd, &state_data.state, state_data.data_len);
                                SELFFUNC_ERR_CHECK(ret, "recv_n");
                                // 转到各个命令的处理函数
                                switch (state_data.state)
                                {
                                case LS:
                                    do_ls(&p->client_info, &state_data);
                                    break;
                                case CD:
                                    do_cd(&p->client_info, &state_data);
                                    break;
                                case RM:
                                    do_rm(&p->client_info, &state_data);
                                    break;
                                case PWD:
                                    do_pwd(&p->client_info);
                                    break;
                                case MKDIR:
                                    do_mkdir(&p->client_info, &state_data);
                                    break;
                                default:
                                    break;
                                }
                                memset(&state_data, 0, sizeof(state_train_t));
                                // 更新和检查轮盘
                                roulette_update_and_check(roulette, &clientInfo_linkList, &j, &past, -1, p->client_info.sfd);
                            }
                        }
                        p = p->next;
                    }
                }
            }
        }
        else
        {
            // 更新和检查轮盘
            roulette_update_and_check(roulette, &clientInfo_linkList, &j, &past, epfd, -1);
        }
    }
    return 0;
}