#include"EventItem.hpp"
#include"Reactor.hpp"
#include"Socket.hpp"
#include"Util.hpp"
#define IP "127.0.0.1"
#define PORT 2000
#define BACK_LOG 5
#define TIMEOUT 1000
int main(int argc,char** argv)
{
    int sfd=Socket::socket_create();
    set_nonblock(sfd);
    Socket::socket_bind(sfd,IP,PORT);
    Socket::socket_listen(sfd,BACK_LOG);
    Reactor reactor;
    reactor.init_reactor();
    EventItem item;
    item.m_fd=sfd;
    item.m_reactor=&reactor;
    item.manageCallbacks(accepter,nullptr,nullptr);
    reactor.add_event(sfd,EPOLLIN|EPOLLET,item);
    while(true)
    {
        reactor.dispatcher(TIMEOUT);
    }
    return 0;
}