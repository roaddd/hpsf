#ifndef __EVENTLOOP_HPP__
#define __EVENTLOOP_HPP__

#include "Epoll.hpp"

namespace hpsf
{
    class Channel;
    class Epoll;

    class EventLoop
    {
        public:
            EventLoop();
            ~EventLoop();

            void loop();
            void update(Channel* Channel);
        private:
            bool quit_;
            Epoll* poller_;
    };
} // namespace hpsf


#endif