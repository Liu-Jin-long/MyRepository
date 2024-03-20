#ifndef __EVENTITEM_HPP__
#define __EVENTITEM_HPP__
#include <iostream>
class EventItem;
typedef int (*callback_t)(EventItem *);//函数类型
class Reactor;
class EventItem
{
public:
    int m_fd;
    Reactor *m_reactor;

    callback_t m_recv_handler;
    callback_t m_send_handler;
    callback_t m_error_handler;
    std::string m_in_buffer;
    std::string m_out_buffer;

public:
    EventItem() : m_fd(-1), m_reactor(nullptr), m_recv_handler(nullptr),
                  m_send_handler(nullptr), m_error_handler(nullptr) {}
    void manageCallbacks(callback_t recv_handler, callback_t send_handler, callback_t error_handler)
    {
        m_recv_handler = recv_handler;
        m_send_handler = send_handler;
        m_error_handler = error_handler;
    }
    ~EventItem() {}
};

#endif