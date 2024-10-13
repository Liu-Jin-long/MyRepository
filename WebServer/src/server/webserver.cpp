#include "webserver.h"
WebServer::WebServer(int port, int trigMode, int timeoutMS, bool OptLinger,
                     int sqlPort, const char *sqlUser, const char *sqlPwd,
                     const char *databaseName, int connPoolNum, int threadNum,
                     bool openLog, Log::Level logLevel, int logQueueSize) : m_port(port), m_openLinger(OptLinger), m_timeoutMS(timeoutMS),
                                                                     m_isClosed(false), m_timer(new HeapTimer()),
                                                                     m_threadPool(new ThreadPool(threadNum)), m_epoller(new Epoller())
{
    m_srcDir = getcwd(nullptr, 256); // 动态内存分配需手动释放
    assert(m_srcDir);
    strncat(m_srcDir, "/resources/", 16);
    HttpConn::userCount = 0;
    HttpConn::srcDir = m_srcDir;
    SqlConnPool::Instance()->Init("localhost", sqlPort, sqlUser, sqlPwd, databaseName, connPoolNum);
    initEventMode(trigMode);
    if (!initSocket())
    {
        m_isClosed = true;
    }
    if (openLog)
    {
        Log::Instance()->init(logLevel, "./log", ".log", logQueueSize);
        if (m_isClosed)
        {
            LOG_ERROR("========== Server init error!==========");
        }
        else
        {
            LOG_INFO("========== Server init ==========");
            LOG_INFO("Port:%d, OpenLinger: %s", m_port, m_openLinger ? "true" : "false");
            LOG_INFO("Listen Mode: %s, OpenConn Mode: %s",
                     (m_listenEvent & EPOLLET ? "ET" : "LT"),
                     (m_connEvent & EPOLLET ? "ET" : "LT"));
            LOG_INFO("LogSys level: %d", logLevel);
            LOG_INFO("srcDir: %s", HttpConn::srcDir);
            LOG_INFO("SqlConnPool num: %d, ThreadPool num: %d", connPoolNum, threadNum);
        }
    }
}
WebServer::~WebServer()
{
    close(m_listenFd);
    m_isClosed = true;
    free(m_srcDir);
    SqlConnPool::Instance()->ClosePool();
}
void WebServer::initEventMode(int trigMode)
{
    m_listenEvent = EPOLLRDHUP;              // 当对端关闭连接时触发
    m_connEvent = EPOLLONESHOT | EPOLLRDHUP; // 一次性事件和关闭事件
    switch (trigMode)
    {
    case 0:
        break;
    case 1:
        m_connEvent |= EPOLLET;
        break;
    case 2:
        m_listenEvent |= EPOLLET;
        break;
    case 3:
        m_listenEvent |= EPOLLET;
        m_connEvent |= EPOLLET;
        break;
    default:
        m_listenEvent |= EPOLLET;
        m_connEvent |= EPOLLET;
        break;
    }
    HttpConn::isET = (m_connEvent & EPOLLET);
}
void WebServer::start()
{
    int timeMS = -1;
    if (!m_isClosed)
    {
        LOG_INFO("========== Server start ==========");
    }
    while (!m_isClosed)
    {
        if (m_timeoutMS > 0)
        {
            // 获取下一个超时时间
            timeMS = m_timer->getNextTick();
        }
        int eventCount = m_epoller->wait(timeMS); // 等待事件发生
        for (int i = 0; i < eventCount; i++)
        {
            int fd = m_epoller->getEventFd(i);
            uint32_t events = m_epoller->getEvents(i);
            if (fd == m_listenFd)
            {
                dealListen();
            }
            else if (events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) // 检查连接是否异常
            {
                assert(m_clients.count(fd) > 0);
                closeConn(&m_clients[fd]);
            }
            else if (events & EPOLLIN)
            {
                assert(m_clients.count(fd) > 0);
                dealRead(&m_clients[fd]);
            }
            else if (events & EPOLLOUT)
            {
                assert(m_clients.count(fd) > 0);
                dealWrite(&m_clients[fd]);
            }
            else
            {
                LOG_ERROR("Unexpected event");
            }
        }
    }
}
void WebServer::sendError(int fd, const char *info)
{
    assert(fd > 0);
    int ret = send(fd, info, strlen(info), 0);
    if (ret < 0)
    {
        LOG_WARN("send error to client[%d] error!", fd);
    }
    close(fd);
}
void WebServer::closeConn(HttpConn *client)
{
    assert(client);
    LOG_INFO("Client[%d] quit!", client->getFd());
    m_epoller->delFd(client->getFd());
    client->Close();
}
void WebServer::addClient(int fd, sockaddr_in addr)
{
    assert(fd > 0);
    m_clients[fd].init(fd, addr);
    if (m_timeoutMS > 0)
    {
        // 客户端连接超时关闭
        m_timer->addNode(fd, m_timeoutMS, std::bind(&WebServer::closeConn, this, &m_clients[fd]));
    }
    m_epoller->addFd(fd, EPOLLIN | m_connEvent);
    setFdNonBlock(fd);
    LOG_INFO("Client[%d] in!", m_clients[fd].getFd());
}
void WebServer::dealListen()
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    do
    {
        int fd = accept(m_listenFd, (struct sockaddr *)&addr, &len);
        if (fd < 0)
        {
            return;
        }
        else if (HttpConn::userCount >= MAX_FD)
        {
            sendError(fd, "Server busy!");
            LOG_WARN("Clients is full!");
            return;
        }
        addClient(fd, addr);
    } while (m_listenEvent & EPOLLET); // 如果是边缘触发，继续接受连接
}
void WebServer::dealRead(HttpConn *client)
{
    assert(client);
    extentTime(client);
    m_threadPool->addTask(std::bind(&WebServer::onRead, this, client));
}
void WebServer::dealWrite(HttpConn *client)
{
    assert(client);
    extentTime(client);
    m_threadPool->addTask(std::bind(&WebServer::onWrite, this, client));
}
void WebServer::extentTime(HttpConn *client)
{
    assert(client);
    if (m_timeoutMS > 0)
    {
        m_timer->adjust(client->getFd(), m_timeoutMS);
    }
}
void WebServer::onRead(HttpConn *client)
{
    assert(client);
    int ret = -1;
    int readErrno = 0;
    ret = client->read(&readErrno);
    if (ret <= 0 && readErrno != EAGAIN)
    {
        closeConn(client);
        return;
    }
    onProcess(client); // 解析客户端请求和准备响应
}
void WebServer::onProcess(HttpConn *client)
{
    // 解析客户端请求和准备响应
    if (client->process())
    {
        m_epoller->modifyFd(client->getFd(), m_connEvent | EPOLLOUT);
    }
    else
    {
        m_epoller->modifyFd(client->getFd(), m_connEvent | EPOLLIN);
    }
}
void WebServer::onWrite(HttpConn *client)
{
    assert(client);
    int ret = -1, writeErrno = 0;
    ret = client->write(&writeErrno);
    if (client->toWriteBytes() == 0) // 要写内容长度
    {
        // 传输完成
        if (client->isKeepAlive())
        {
            onProcess(client); // 处理后续请求
            return;
        }
    }
    else if (ret < 0)
    {
        if (writeErrno == EAGAIN) // 如果写入时没有数据可发送
        {
            // 继续传输
            m_epoller->modifyFd(client->getFd(), m_connEvent | EPOLLOUT); // 重新修改事件为 EPOLLOUT 继续发送
            return;
        }
    }
    closeConn(client);
}
bool WebServer::initSocket()
{
    int ret;
    struct sockaddr_in addr;
    if (m_port > 65536 || m_port < 1024)
    {
        LOG_ERROR("Port:%d error!", m_port);
        return false;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(m_port);
    struct linger optLinger = {0}; // linger 结构体用于设置关闭 Socket 的策略
    if (m_openLinger)
    {
        // 优雅关闭: 直到所剩数据发送完毕或超时
        optLinger.l_onoff = 1;
        optLinger.l_linger = 1;
    }
    m_listenFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listenFd < 0)
    {
        LOG_ERROR("Create socket error!");
        return false;
    }
    ret = setsockopt(m_listenFd, SOL_SOCKET, SO_LINGER, &optLinger, sizeof(optLinger));
    if (ret < 0)
    {
        close(m_listenFd);
        LOG_ERROR("Set linger error!");
        return false;
    }
    int optval = 1;
    ret = setsockopt(m_listenFd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
    if (ret < 0)
    {
        LOG_ERROR("set socket setsockopt SO_REUSEADDR error !");
        close(m_listenFd);
        return false;
    }
    ret = bind(m_listenFd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        LOG_ERROR("bind error!");
        close(m_listenFd);
        return false;
    }
    // listen函数将主动连接套接字转换为被连接套接字，用于服务器进程接收连接请求。
    ret = listen(m_listenFd, 6); // 服务器最多允许同时存在backlog+1个处于ESTABLISHED状态的连接
    if (ret < 0)
    {
        LOG_ERROR("listen error!");
        close(m_listenFd);
        return false;
    }
    ret = m_epoller->addFd(m_listenFd, m_listenEvent | EPOLLIN);
    if (ret == 0)
    {
        LOG_ERROR("Add m_listenFd error!");
        close(m_listenFd);
        return false;
    }
    setFdNonBlock(m_listenFd); // 设置监听 Socket 为非阻塞模式
    LOG_INFO("Server port:%d", m_port);
    return true;
}
int WebServer::setFdNonBlock(int fd)
{
    assert(fd > 0);
    return fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0) | O_NONBLOCK);
}