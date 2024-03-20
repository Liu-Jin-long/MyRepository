#ifndef __UTIL_HPP__
#define __UTIL_HPP__
#include "EventItem.hpp"
#include "Reactor.hpp"
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
struct data
{
    int x;
    int y;
    char op;
};
class StringUtil
{
public:
    static void split(std::string &in, std::vector<std::string> &out, std::string sep)
    {
        int start = 0;
        size_t position = in.find(sep, start);
        while (position != std::string::npos)
        {
            out.push_back(in.substr(start, position - start));
            start = position + sep.size();
            position = in.find(sep, start);
        }
        in = in.substr(start);
    }
    static void deserialize(std::string &in, data &d)
    {
        size_t pos;
        for (pos = 0; pos < in.size(); pos++)
        {
            if (in[pos] == '+' || in[pos] == '-' || in[pos] == '*' || in[pos] == '/' || in[pos] == '%')
            {
                break;
            }
        }
        if (pos < in.size())
        {
            std::string left = in.substr(0, pos);
            std::string right = in.substr(pos + 1);
            d.x = atoi(left.c_str());
            d.y = atoi(right.c_str());
            d.op = in[pos];
        }
        else
        {
            d.op = -1;
        }
    }
};
int set_nonblock(int fd)
{
    int fl = fcntl(fd, F_GETFL);
    if (fl < 0)
    {
        std::cerr << "accept error at file:" << __FILE__ << " ,line:" << __LINE__ << std::endl;
        return -1;
    }
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
    return 0;
}

int recv_helper(int socket_fd, std::string &out)
{
    while (true)
    {
        char buf[128] = {0};
        ssize_t size = recv(socket_fd, buf, sizeof(buf) - 1, 0);
        if (size < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return 0;
            }
            else if (errno == EINTR)
            {
                continue;
            }
            else
            {
                return -1;
            }
        }
        else if (size == 0)
        {
            return -1;
        }
        out += buf;
        return 0;
    }
}
int recver(EventItem *item)
{
    if (item->m_fd < 0)
    {
        return -1;
    }
    int ret = recv_helper(item->m_fd, item->m_in_buffer);
    if (-1 == ret)
    {
        item->m_error_handler(item);
        return -1;
    }
    std::vector<std::string> datagrams;
    StringUtil::split(item->m_in_buffer, datagrams, " ");
    for (auto s : datagrams)
    {
        struct data d = {0};
        StringUtil::deserialize(s, d);
        int ret;
        switch (d.op)
        {
        case '+':
            ret = d.x + d.y;
            break;
        case '-':
            ret = d.x - d.y;
            break;
        case '*':
            ret = d.x * d.y;
            break;
        case '/':
            if (0 == d.y)
            {
                std::cerr << "Error: div zero!" << std::endl;
                continue; //除0错误 处理下一个
            }
            else
            {
                ret = d.x / d.y;
            }
            break;
        case '%':
            if (0 == d.y)
            {
                std::cerr << "Error: mod zero!" << std::endl;
                continue; //模0错误 处理下一个
            }
            else
            {
                ret = d.x % d.y;
            }
            break;
        default:
            std::cerr << "operation error!" << std::endl;
            continue; //继续处理下一个报文
        }

        std::string response;
        response += std::to_string(d.x);
        response += d.op;
        response += std::to_string(d.y);
        response += '=';
        response += std::to_string(ret);
        response += ' ';
        item->m_out_buffer += response;
        if (!item->m_out_buffer.empty())
        {
            item->m_reactor->enable_read_write(item->m_fd, true, true);
        }
    }
    return 0;
}
int sender_helper(int fd, std::string &in)
{
    size_t total = 0;
    while (true)
    {
        ssize_t size = send(fd, in.c_str() + total, in.size() - total, 0);
        if (size < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                in.erase(0, total);
                return 1; //发送缓冲区满
            }
            else if (errno == EINTR)
            {
                continue; //被信号中断返回
            }
            else
            {
                return -1;
            }
        }
        total += size;
        if (total >= in.size())
        {
            in.clear();
            return 0;
        }
    }
}
int sender(EventItem *item)
{
    if (item->m_fd < 0)
    {
        return -1;
    }
    int ret = sender_helper(item->m_fd, item->m_out_buffer);
    if (ret == 0)
    {
        item->m_reactor->enable_read_write(item->m_fd, true, false);
    }
    else if (ret == 1)
    {
        item->m_reactor->enable_read_write(item->m_fd, true, true);
    }
    else
    {
        item->m_error_handler(item);
    }
    return 0;
}
int errorer(EventItem *item)
{
    item->m_reactor->delete_event(item->m_fd);
    close(item->m_fd);
    
    item->m_fd = -1;
    return 0;
}
int accepter(EventItem *item)
{
    while (true)
    {
        struct sockaddr_in peer = {0};
        socklen_t len = sizeof(peer);
        int socket_fd = accept(item->m_fd, (struct sockaddr *)&peer, &len);
        if (socket_fd < 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                return 0;
            }
            else if (errno == EINTR)
            {
                continue;
            }
            else
            {
                std::cerr << "accept error at file:" << __FILE__ << " ,line:" << __LINE__ << std::endl;
                return -1;
            }
        }
        set_nonblock(socket_fd);
        EventItem socket_item;
        socket_item.m_fd = socket_fd;
        socket_item.m_reactor = item->m_reactor;
        socket_item.manageCallbacks(recver, sender, errorer);
        item->m_reactor->add_event(socket_fd, EPOLLIN | EPOLLET, socket_item);
    }
    return 0;
}
#endif