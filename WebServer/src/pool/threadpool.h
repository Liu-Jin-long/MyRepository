#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <functional>
#include <assert.h>
class ThreadPool
{
public:
    explicit ThreadPool(size_t threadCount = 8) : m_pool(std::make_shared<Pool>())
    {
        assert(threadCount > 0);
        m_pool->isClosed = false;
        for (size_t i = 0; i < threadCount; i++)
        {
            std::thread([pool = m_pool]
                        { std::unique_lock<std::mutex> locker(pool->mutex);
                        while(true)
                        {
                            // Wait until there is a task or the pool is closed
                            if(pool->taskQueue.empty() && !pool->isClosed)
                            {
                                pool->cond.wait(locker);
                            }
                            if (pool->isClosed && pool->taskQueue.empty()) 
                            {  
                                break;  // Exit if pool is closed and no tasks are left  
                            }
                            if (!pool->taskQueue.empty()) 
                            {  
                                auto task = std::move(pool->taskQueue.front());  
                                pool->taskQueue.pop();  
                                locker.unlock();  // Unlock before executing the task   
                                task();  // Execute the task  
                                locker.lock();  // Lock again after task execution 
                            } 
                        }})
                .detach();
        }
    }
    ThreadPool() = default;
    ThreadPool(ThreadPool &&) = default;
    ~ThreadPool()
    {
        if (static_cast<bool>(m_pool))
        {
            {
                std::lock_guard<std::mutex> locker(m_pool->mutex);
                m_pool->isClosed = true;
            }
            m_pool->cond.notify_all();
        }
    }
    template <typename T>
    void addTask(T &&task)
    {
        {
            std::lock_guard<std::mutex> locker(m_pool->mutex);
            m_pool->taskQueue.emplace(std::forward<T>(task));
        }
        m_pool->cond.notify_one();
    }

private:
    struct Pool
    {
        std::mutex mutex;
        std::condition_variable cond;
        bool isClosed;
        std::queue<std::function<void()>> taskQueue;
    };
    std::shared_ptr<Pool> m_pool;
};
#endif