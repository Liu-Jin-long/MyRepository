#include "buffer.h"
Buffer::Buffer(int initBuffSize) : m_buffer(initBuffSize), m_readPos(0), m_writePos(0) {}
size_t Buffer::ReadableBytes() const
{
    return m_writePos - m_readPos;
}
size_t Buffer::WritableBytes() const
{
    return m_buffer.size() - m_writePos;
}
size_t Buffer::PrependableBytes() const
{
    return m_readPos;
}
void Buffer::AddWritePos(size_t len)
{
    m_writePos += len;
}
const char *Buffer::Peek() const
{
    return BeginPtr() + m_readPos;
}
void Buffer::Retrieve(size_t len)
{
    assert(len <= ReadableBytes());
    m_readPos += len;
}
void Buffer::RetrieveUntil(const char *end)
{
    assert(Peek() <= end);
    Retrieve(end - Peek());
}
void Buffer::RetrieveAll()
{
    bzero(&m_buffer[0], m_buffer.size());
    m_readPos = m_writePos = 0;
}
std::string Buffer::RetrieveAllToStr()
{
    std::string str(Peek(), ReadableBytes());
    RetrieveAll();
    return str;
}
const char *Buffer::BeginWriteConst() const
{
    return BeginPtr() + m_writePos;
}
char *Buffer::BeginWrite()
{
    return BeginPtr() + m_writePos;
}
void Buffer::Append(const std::string &str)
{
    Append(str.data(), str.length());
}
void Buffer::Append(const char *str, size_t len)
{
    assert(str);
    EnsureWriteable(len);
    std::copy(str, str + len, BeginWrite());
    m_writePos += len;
}
void Buffer::Append(const void *data, size_t len)
{
    assert(data);
    Append(static_cast<const char *>(data), len);
}
void Buffer::Append(const Buffer &buffer)
{
    Append(buffer.Peek(), buffer.ReadableBytes());
}
void Buffer::EnsureWriteable(size_t len)
{
    if (WritableBytes() < len)
    {
        MakeSpace(len);
    }
    assert(WritableBytes() >= len);
}
ssize_t Buffer::ReadFd(int fd, int *Errno)
{
    char buff[65535];
    // POSIX标准定义的数据结构，通常用于描述一个或多个缓冲区的地址和长度。
    struct iovec iov[2];
    const size_t writable = WritableBytes();
    // iov[0]首先能写入的缓冲区描述信息
    iov[0].iov_base = BeginPtr() + m_writePos;
    iov[0].iov_len = writable;
    // iov[1]接收超出iov[0]缓冲区的额外数据
    iov[1].iov_base = buff;
    iov[1].iov_len = sizeof(buff);

    const ssize_t len = readv(fd, iov, 2);
    if (len < 0)
    {
        *Errno = errno;
    }
    else if (static_cast<size_t>(len) <= writable)
    {
        m_writePos += len;
    }
    else
    {
        m_writePos = m_buffer.size();
        Append(buff, len - writable);
    }
    return len;
}
ssize_t Buffer::WriteFd(int fd, int *Errno)
{
    ssize_t len = write(fd, Peek(), ReadableBytes());
    if (len < 0)
    {
        *Errno = errno;
        return len;
    }
    m_readPos += len;
    return len;
}
char *Buffer::BeginPtr()
{
    return &*m_buffer.begin();
}
const char *Buffer::BeginPtr() const
{
    return &*m_buffer.begin();
}
void Buffer::MakeSpace(size_t len)
{
    if (WritableBytes() + PrependableBytes() < len)
    {
        m_buffer.resize(m_writePos + len + 1);
    }
    else
    {
        size_t readable = ReadableBytes();
        // 将缓冲区未读内容移到缓冲区起始位置
        std::copy(BeginPtr() + m_readPos, BeginPtr() + m_writePos, BeginPtr());
        m_readPos = 0;
        m_writePos = m_readPos + readable;
        assert(readable == ReadableBytes());
    }
}