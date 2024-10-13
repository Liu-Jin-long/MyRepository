#include "heaptimer.h"
void HeapTimer::adjustUp(size_t index)
{
    assert(index >= 0 && index < m_heap.size());
    size_t j = (index - 1) / 2;
    while (j >= 0)
    {
        if (m_heap[j] < m_heap[index])
        {
            break;
        }
        swapNode(index, j);
        index = j;
        j = (index - 1) / 2;
    }
}
void HeapTimer::swapNode(size_t i, size_t j)
{
    assert(i >= 0 && i < m_heap.size());
    assert(j >= 0 && j < m_heap.size());
    std::swap(m_heap[i], m_heap[j]);
    m_ref[m_heap[i].id] = j;
    m_ref[m_heap[j].id] = i;
}
bool HeapTimer::adjustDown(size_t index)
{
    assert(index >= 0 && index < m_heap.size());
    size_t n = m_heap.size();
    size_t i = index;
    size_t j = i * 2 + 1;
    while (j < n)
    {
        if (j + 1 < n && m_heap[j + 1] < m_heap[j])
        {
            j++;
        }
        if (m_heap[i] < m_heap[j])
        {
            break;
        }
        swapNode(i, j);
        i = j;
        j = i * 2 + 1;
    }
    return i > index;
}
void HeapTimer::addNode(int id, int timeout, const TimeoutCallback &cb)
{
    assert(id >= 0);
    size_t i;
    if (m_ref.count(id) == 0)
    {
        // 堆尾添加新节点，向上调整堆
        i = m_heap.size();
        m_ref[id] = i;
        m_heap.push_back({id, Clock::now() + MS(timeout), cb});
        adjustUp(i);
    }
    else
    {
        // 已有该节点下，修改并调整
        i = m_ref[id];
        m_heap[i].expires = Clock::now() + MS(timeout);
        m_heap[i].cb = cb;
        if (!(adjustDown(i)))
        {
            adjustUp(i);
        }
    }
}
void HeapTimer::dowork(int id)
{
    // 删除指定id结点，触发回调函数
    if (m_heap.empty() || m_ref.count(id) == 0)
    {
        return;
    }
    size_t i = m_ref[id];
    TimerNode node = m_heap[i];
    node.cb();
    delNode(i);
}
void HeapTimer::delNode(size_t index)
{
    assert(!m_heap.empty() && index >= 0 && index < m_heap.size());
    size_t i = index;
    size_t n = m_heap.size() - 1;
    if (i < n)
    {
        swapNode(i, n);
        if (!(adjustDown(i)))
        {
            adjustUp(i);
        }
    }
    m_ref.erase(m_heap.back().id);
    m_heap.pop_back();
}
void HeapTimer::adjust(int id, int timeout)
{
    // 调整指定id的结点
    assert(!m_heap.empty() && m_ref.count(id) > 0);
    m_heap[m_ref[id]].expires = Clock::now() + MS(timeout);
    if (!(adjustDown(m_ref[id])))
    {
        adjustUp(m_ref[id]);
    }
}
void HeapTimer::tick()
{
    if (m_heap.empty())
    {
        return;
    }
    while (!m_heap.empty())
    {
        TimerNode node = m_heap.front();
        if (std::chrono::duration_cast<MS>(node.expires - Clock::now()).count() > 0)
        {
            break;
        }
        node.cb();
        popNode();
    }
}
void HeapTimer::popNode()
{
    assert(!m_heap.empty());
    delNode(0);
}
void HeapTimer::clear()
{
    m_ref.clear();
    m_heap.clear();
}
int HeapTimer::getNextTick()
{
    tick();
    size_t res = -1;
    if (!m_heap.empty())
    {
        res = std::chrono::duration_cast<MS>(m_heap.front().expires - Clock::now()).count();
        if (res < 0)
        {
            res = 0;
        }
    }
    return res;
}