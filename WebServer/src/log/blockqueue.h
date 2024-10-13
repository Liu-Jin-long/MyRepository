#ifndef __BLOCKQUEUE_H__
#define __BLOCKQUEUE_H__
#include <mutex>
#include <deque>
#include <condition_variable>
#include <sys/time.h>
#include <assert.h>
// 阻塞队列
template <typename T>
class BlockQueue
{
public:
    explicit BlockQueue(size_t MaxCapacity = 1000);
    ~BlockQueue();
    void clear();
    bool empty();
    bool full();
    void close();
    bool isClosed();
    size_t size();
    size_t capacity();
    T front();
    T back();
    void push_back(const T &item);
    void push_front(const T &item);
    bool pop(T &i);
    bool pop(T &item, int timeout);
    void flush(); // 唤醒一个阻塞的消费者取队列资源

private:
    std::deque<T> m_deque;
    size_t m_capacity;
    bool m_isclosed;
    std::mutex m_mutex;
    std::condition_variable m_consumer;
    std::condition_variable m_producer;
};
template <typename T>
BlockQueue<T>::BlockQueue(size_t MaxCapacity) : m_capacity(MaxCapacity)
{
    assert(MaxCapacity > 0);
    m_isclosed = false;
}
template <typename T>
bool BlockQueue<T>::isClosed()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_isclosed;
}
template <typename T>
BlockQueue<T>::~BlockQueue()
{
    close();
}
template <typename T>
void BlockQueue<T>::close()
{
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        m_deque.clear();
        m_isclosed = true;
    }
    m_producer.notify_all();
    m_consumer.notify_all();
}
template <typename T>
void BlockQueue<T>::flush()
{
    m_consumer.notify_one();
}
template <typename T>
void BlockQueue<T>::clear()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    m_deque.clear();
}
template <typename T>
T BlockQueue<T>::front()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_deque.front();
}
template <typename T>
T BlockQueue<T>::back()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_deque.back();
}
template <typename T>
size_t BlockQueue<T>::size()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_deque.size();
}
template <typename T>
size_t BlockQueue<T>::capacity()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_capacity;
}
template <typename T>
void BlockQueue<T>::push_back(const T &item)
{
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_deque.size() >= m_capacity)
    {
        m_producer.wait(locker);
    }
    m_deque.push_back(item);
    m_consumer.notify_one();
}
template <typename T>
void BlockQueue<T>::push_front(const T &item)
{
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_deque.size() >= m_capacity)
    {
        m_producer.wait(locker);
    }
    m_deque.push_front(item);
    m_consumer.notify_one();
}

template <typename T>
bool BlockQueue<T>::empty()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_deque.empty();
}
template <typename T>
bool BlockQueue<T>::full()
{
    std::lock_guard<std::mutex> locker(m_mutex);
    return m_deque.size() >= m_capacity;
}
template <typename T>
bool BlockQueue<T>::pop(T &item)
{
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_deque.empty())
    {
        if (m_isclosed)
        {
            return false;
        }
        m_consumer.wait(locker);
    }
    item = m_deque.front();
    m_deque.pop_front();
    m_producer.notify_one();
    return true;
}
template <typename T>
bool BlockQueue<T>::pop(T &item, int timeout)
{
    std::unique_lock<std::mutex> locker(m_mutex);
    while (m_deque.empty())
    {
        if (timeout == -1)
        {
            m_consumer.wait(locker);
        }
        else
        {
            if (m_consumer.wait_for(locker, std::chrono::seconds(timeout)) == std::cv_status::timeout)
            {
                return false;
            }
        }
        if (m_isclosed)
        {
            return false;
        }
    }
    item = m_deque.front();
    m_deque.pop_front();
    m_producer.notify_one();
    return true;
}
#endif