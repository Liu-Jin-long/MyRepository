#include "httpconn.h"
const char *HttpConn::srcDir;
std::atomic<int> HttpConn::userCount;
bool HttpConn::isET;
HttpConn::HttpConn()
{
    m_sfd = -1;
    m_clientAddr = {0};
    m_isClosed = true;
}
HttpConn::~HttpConn()
{
    Close();
}
void HttpConn::init(int sfd, const sockaddr_in &clientAddr)
{
    assert(sfd >= 0);
    userCount++;
    m_clientAddr = clientAddr;
    m_sfd = sfd;
    m_writeBuffer.RetrieveAll();
    m_readBuffer.RetrieveAll();
    m_isClosed = false;
    LOG_INFO("Client[%d](%s:%d) in, userCount:%d", m_sfd, getIP(), getPort(), (int)userCount);
}
void HttpConn::Close()
{
    m_response.unmapFile();
    if (!m_isClosed)
    {
        m_isClosed = true;
        userCount--;
        close(m_sfd);
        LOG_INFO("Client[%d](%s:%d) quit, UserCount:%d", m_sfd, getIP(), getPort(), (int)userCount);
    }
}
int HttpConn::getFd() const
{
    return m_sfd;
}
struct sockaddr_in HttpConn::getAddr() const
{
    return m_clientAddr;
}
const char *HttpConn::getIP() const
{
    return inet_ntoa(m_clientAddr.sin_addr);
}
int HttpConn::getPort() const
{
    return ntohs(m_clientAddr.sin_port);
}
ssize_t HttpConn::read(int *saveErrno)
{
    ssize_t len = -1;
    do
    {
        len = m_readBuffer.ReadFd(m_sfd, saveErrno);
        if (len <= 0)
        {
            break;
        }
    } while (isET);
    return len;
}
ssize_t HttpConn::write(int *saveErrno)
{
    ssize_t len = 0, total = 0;
    do
    {
        len = writev(m_sfd, m_iov, m_iovCount);
        if (len <= 0)
        {
            *saveErrno = errno;
            return -1;
        }
        total += len;

        if (m_iov[0].iov_len + m_iov[1].iov_len == 0)
        {
            // 所有数据已发送完毕，退出循环
            break;
        }
        else if (static_cast<size_t>(len) > m_iov[0].iov_len)
        {
            // 如果写入字节数大于第一个iovec中的数据长度
            m_iov[1].iov_base = (uint8_t *)m_iov[1].iov_base + (len - m_iov[0].iov_len);
            m_iov[1].iov_len -= (len - m_iov[0].iov_len);
            if (m_iov[0].iov_len)
            {
                m_writeBuffer.RetrieveAll();
                m_iov[0].iov_len = 0;
            }
        }
        else
        {
            m_iov[0].iov_base = (uint8_t *)m_iov[0].iov_base + len;
            m_iov[0].iov_len -= len;
            m_writeBuffer.Retrieve(len);
        }

    } while (isET || toWriteBytes() > 10240);
    return total;
}
bool HttpConn::process()
{
    m_request.init();
    if (m_readBuffer.ReadableBytes() <= 0)
    {
        return false;
    }
    else if (m_request.parse(m_readBuffer))
    {
        LOG_DEBUG("%s", m_request.path().c_str());
        m_response.init(srcDir, m_request.path(), m_request.isKeepAlive(), 200);
    }
    else
    {
        m_response.init(srcDir, m_request.path(), false, 400);
    }
    m_response.makeResponse(m_writeBuffer);
    m_iov[0].iov_base = const_cast<char *>(m_writeBuffer.Peek());
    m_iov[0].iov_len = m_writeBuffer.ReadableBytes();
    m_iovCount = 1;
    if (m_response.fileLen() > 0 && m_response.file())
    {
        m_iov[1].iov_base = m_response.file();
        m_iov[1].iov_len = m_response.fileLen();
        m_iovCount = 2;
    }
    LOG_DEBUG("filesize:%d, %d  to %d", m_response.fileLen(), m_iovCount, toWriteBytes())
    return true;
}