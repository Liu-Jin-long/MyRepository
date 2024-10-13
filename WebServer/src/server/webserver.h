#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__
#include <unordered_map>
#include <fcntl.h>  // fcntl()
#include <unistd.h> // close()
#include <assert.h>
#include <errno.h>
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h>  // inet_ntoa()

#include "epoller.h"
#include "../log/log.h"
#include "../timer/heaptimer.h"
#include "../pool/sqlconnpool.h"
#include "../pool/threadpool.h"
#include "../pool/sqlconnRAII.h"
#include "../http/httpconn.h"
class WebServer
{
public:
    WebServer(int port, int trigMode, int timeoutMS, bool OptLinger,
              int sqlPort, const char *sqlUser, const char *sqlPwd,
              const char *databaseName, int connPoolNum, int threadNum,
              bool openLog, Log::Level logLevel, int logQueueSize);
    ~WebServer();
    void start();

private:
    bool initSocket();
    void initEventMode(int trigMode);
    void addClient(int fd, sockaddr_in addr);
    void dealListen();
    void dealWrite(HttpConn *client);
    void dealRead(HttpConn *client);
    void sendError(int fd, const char *info);
    void extentTime(HttpConn *client);
    void closeConn(HttpConn *client);
    void onRead(HttpConn *client);
    void onWrite(HttpConn *client);
    void onProcess(HttpConn *client);
    static const int MAX_FD = 65536;
    static int setFdNonBlock(int fd);
    int m_port;        // 服务器端口
    bool m_openLinger; // 优雅关闭
    int m_timeoutMS;   // epoll超时时间
    bool m_isClosed;   // 服务器是否关闭
    int m_listenFd;   // 监听客户端连接的文件描述符
    char *m_srcDir;    // 静态资源目录
    uint32_t m_listenEvent;
    uint32_t m_connEvent;
    std::unique_ptr<HeapTimer> m_timer;          // 定时器处理非活动连接
    std::unique_ptr<ThreadPool> m_threadPool;    // 线程池
    std::unique_ptr<Epoller> m_epoller;          // epoll 事件处理器
    std::unordered_map<int, HttpConn> m_clients; // socket文件描述符和客户端连接映射
};
#endif