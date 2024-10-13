#ifndef __BUFFER_H__
#define __BUFFER_H__
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/uio.h>
#include <vector>
#include <atomic>
#include <assert.h>
//   prependable      readable       writable
// ---------------------------------------------
// |             |    content    |             |
// ---------------------------------------------
// 0          readPos          writePos      size()
class Buffer
{
public:
    Buffer(int initBufferSize = 1024);
    ~Buffer() = default;
    size_t WritableBytes() const;    // 可写空间
    size_t ReadableBytes() const;    // 刻度空间
    size_t PrependableBytes() const; // 可添加部分
    const char *Peek() const;        // 缓冲区读指针位置
    void EnsureWriteable(size_t len);
    void AddWritePos(size_t len);        // 以指针方式写时移动m_writePos
    void Retrieve(size_t len);           // 清除len长度的内容
    void RetrieveUntil(const char *end); // 清除内容直到结束指针位置
    void RetrieveAll();                  // 清除缓冲区内容
    std::string RetrieveAllToStr();      // 获取缓冲区所有未读内容并清空
    const char *BeginWriteConst() const; // 缓冲区写指针位置
    char *BeginWrite();                  // 缓冲区写位置指针
    void Append(const std::string &str);
    void Append(const char *str, size_t len);
    void Append(const void *data, size_t len);
    void Append(const Buffer &buffer);
    ssize_t ReadFd(int fd, int *Errno);
    ssize_t WriteFd(int fd, int *Errno);

private:
    char *BeginPtr(); // 缓冲区起始指针
    const char *BeginPtr() const;
    void MakeSpace(size_t len);
    std::vector<char> m_buffer;
    std::atomic<std::size_t> m_readPos;
    std::atomic<std::size_t> m_writePos;
};

#endif