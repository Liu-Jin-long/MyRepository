#ifndef __HTTPCONN_H__
#define __HTTPCONN_H__
#include <sys/types.h>
#include <sys/uio.h>   // readv/writev
#include <arpa/inet.h> // sockaddr_in
#include <stdlib.h>    // atoi()
#include <errno.h>
#include "../log/log.h"
#include "../pool/sqlconnRAII.h"
#include "../buffer/buffer.h"
#include "httprequest.h"
#include "httpresponse.h"
class HttpConn
{
public:
    HttpConn();
    ~HttpConn();
    void init(int sfd, const sockaddr_in &addr);
    ssize_t read(int *saveErrno);
    ssize_t write(int *saveErrno);
    void Close();
    int getFd() const;
    int getPort() const;
    const char *getIP() const;
    sockaddr_in getAddr() const;
    bool process();
    int toWriteBytes()
    {
        return m_iov[0].iov_len + m_iov[1].iov_len;
    }
    bool isKeepAlive() const
    {
        return m_request.isKeepAlive();
    }
    static bool isET;
    static const char *srcDir;
    static std::atomic<int> userCount;

private:
    int m_sfd;
    struct sockaddr_in m_clientAddr;
    bool m_isClosed;
    int m_iovCount;
    // iovec结构，支持散布读（scatter read）和聚集写（gather write）
    struct iovec m_iov[2];
    Buffer m_readBuffer;
    Buffer m_writeBuffer;
    HttpRequest m_request;
    HttpResponse m_response;
};
#endif