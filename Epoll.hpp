#ifndef __EPOLL_HPP__
#define __EPOLL_HPP__

#include "Channel.hpp"
#include <vector>
#include "Define.hpp"
#include <sys/epoll.h>

namespace hpsf
{
    class Channel;
    
    class Epoll
    {
        public:
            Epoll();
            ~Epoll();

            void update(Channel* channel);
            void poll(std::vector<Channel*>* pChannels);

        private:
            int epollfd_;
            struct epoll_event events_[MAX_EVENTS];
    };
    
} // namespace hpsf


#endif