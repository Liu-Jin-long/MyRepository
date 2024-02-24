#ifndef __CONFIG_H__
#define __CONFIG_H__
//服务器ip地址
#define IP "127.0.0.1"
//服务器端口号
#define PORT "2000"
//服务器端文件池路径
#define FILE_POOL_PATH "../resource/"
//服务器端子线程数量
#define THREAD_NUM 5
//服务器端子线程池容量
#define THREAD_POOL_CAPACITY 10
//服务器端epoll监控客户端socket的最大事件数目 最多同时连接的客户端数目
#define MAX_EVENTS 32
//客户端超时未响应时服务器主动断开的时间(单位:秒)
#define TIMEOUT_DISCONNECT_SECOND 30
#endif