#ifndef __EPOLL_HPP__
#define __EPOLL_HPP__

#include "net/Channel.hpp"
#include "../../../Define.hpp"

#include <sys/epoll.h>
#include <vector>

namespace co
{
    class Epoll
    {
    public:
        Epoll();
        ~Epoll();

        void update(Channel *channel);
        // void update_co()
        void poll(std::vector<Channel *> *pChannels);

    private:
        int epollfd_;
        struct epoll_event events_[MAX_EVENTS];
    };
} // namespace co

#endif