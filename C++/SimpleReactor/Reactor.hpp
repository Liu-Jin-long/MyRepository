#ifndef __REACTOR_HPP__
#define __REACTOR_HPP__
#include <iostream>
#include <unordered_map>
#include <sys/epoll.h>
#define MAX_NUM 64
#define SIZE 256
#include "EventItem.hpp"
#include <unistd.h>
class Reactor
{
private:
    int m_epfd;
    std::unordered_map<int, EventItem> m_event_items;

public:
    Reactor() : m_epfd(-1) {}
    void init_reactor()
    {
        m_epfd = epoll_create(SIZE);
        if (m_epfd == -1)
        {
            std::cerr << "epoll_create error" << std::endl;
            exit(5);
        }
    }
    ~Reactor()
    {
        if (m_epfd >= 0)
        {
            close(m_epfd);
        }
    }
    void add_event(int fd, uint32_t event_type, const EventItem &item)
    {
        struct epoll_event event = {0};
        event.data.fd = fd;
        event.events = event_type;
        int ret = epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &event);
        if (ret != 0)
        {
            std::cerr << "epoll_ctl add error at file:" << __FILE__ << " ,line:" << __LINE__ << std::endl;
        }
        else
        {
            m_event_items.insert({fd, item});
            std::cout << "成功添加fd=" << fd << "到epoll监控中" << std::endl;
        }
    }
    void delete_event(int fd)
    {
        int ret = epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, nullptr);
        if (-1 == ret)
        {
            std::cerr << "epoll_ctl del error, fd: " << fd << std::endl;
        }
        else
        {
            m_event_items.erase(fd);
            std::cout << "成功从epoll监控中删除fd=" << fd << std::endl;
        }
    }
    void enable_read_write(int fd, bool read, bool write)
    {
        struct epoll_event event = {0};
        event.data.fd = fd;
        event.events = (read ? EPOLLIN : 0) | (write ? EPOLLOUT : 0);
        int ret = epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &event);
        if (ret != 0)
        {
            std::cerr << "epoll_ctl mod error at file:" << __FILE__ << " ,line:" << __LINE__ << std::endl;
        }
    }
    void dispatcher(int timeout)
    {
        struct epoll_event ret_events[MAX_NUM] = {0};
        int ready_num = epoll_wait(m_epfd, ret_events, MAX_NUM, timeout);
        int ready_fd;
        for (int i = 0; i < ready_num; i++)
        {
            ready_fd = ret_events->data.fd;
            if ((ret_events[i].events == EPOLLERR) || (ret_events[i].events == EPOLLHUP))
            {
                if (m_event_items[ready_fd].m_error_handler)
                {
                    m_event_items[ready_fd].m_error_handler(&m_event_items[ready_fd]);
                }
            }
            if (ret_events[i].events == EPOLLIN)
            {
                if (m_event_items[ready_fd].m_recv_handler)
                {
                    m_event_items[ready_fd].m_recv_handler(&m_event_items[ready_fd]);
                }
            }
            if (ret_events[i].events == EPOLLOUT)
            {
                if (m_event_items[ready_fd].m_send_handler)
                {
                    m_event_items[ready_fd].m_send_handler(&m_event_items[ready_fd]);
                }
            }
        }
    }
};
#endif