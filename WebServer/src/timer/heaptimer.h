#ifndef __HEAPTIMER_h__
#define __HEAPTIMER_h__
#include <queue>
#include <unordered_map>
#include <time.h>
#include <algorithm>
#include <arpa/inet.h>
#include <functional>
#include <chrono>
#include <assert.h>
#include "../log/log.h"

typedef std::function<void()> TimeoutCallback;
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::milliseconds MS;
typedef Clock::time_point TimeStamp;
struct TimerNode
{
    int id;
    TimeStamp expires;
    TimeoutCallback cb;
    // 小根堆，较小的超时时间优先
    bool operator<(const TimerNode &t)
    {
        return expires < t.expires;
    }
};
// 基于堆的定时器，使用优先队列（最小堆）来管理定时任务
class HeapTimer
{
public:
    HeapTimer(size_t capacity = 64) { m_heap.reserve(capacity); }
    ~HeapTimer() { clear(); }
    void adjust(int id, int timeout);// 调整定时器
    void addNode(int id, int timeout, const TimeoutCallback &cb);// 添加定时器节点
    void dowork(int id);// 执行指定ID的定时器任务
    void clear();// 清理所有定时器
    void tick();// 检查并执行到期的定时器
    void popNode();// 弹出小根堆顶节点
    int getNextTick();// 获取下一个到期时间

private:
    void delNode(size_t index);// 删除指定索引的节点
    void adjustUp(size_t index);// 向上调整堆
    bool adjustDown(size_t index);// 向下调整堆 
    void swapNode(size_t i, size_t j);// 交换两个节点
    std::vector<TimerNode> m_heap;// 存储定时器节点的堆
    // TimerNode的id和堆m_heap索引的映射
    std::unordered_map<int, size_t> m_ref;
};
#endif