#ifndef __EPOLLER_H__
#define __EPOLLER_H__

#include <sys/epoll.h> //epoll_ctl()
#include <fcntl.h>  // fcntl()
#include <unistd.h> // close()
#include <assert.h>
#include <vector>
#include <errno.h>
class Epoller
{
public:
    explicit Epoller(int maxEvent = 1024);
    ~Epoller();
    bool addFd(int fd, uint32_t events);
    bool modifyFd(int fd, uint32_t events);
    bool delFd(int fd);
    int wait(int timeoutMs = -1);
    int getEventFd(size_t i) const;
    uint32_t getEvents(size_t i) const;
private:
    int m_epollFd;
    std::vector<struct epoll_event> m_events;
};

#endif