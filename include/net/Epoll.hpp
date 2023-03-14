#ifndef __EPOLL_HPP__
#define __EPOLL_HPP__

#include "Channel.hpp"
#include "../base/Define.hpp"


#include <vector>
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
            //void update_co()
            void poll(std::vector<Channel*>* pChannels);

        private:
            int epollfd_;
            struct epoll_event events_[MAX_EVENTS];
    };
    
} // namespace hpsf


#endif