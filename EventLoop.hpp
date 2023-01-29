#ifndef __EVENTLOOP_HPP__
#define __EVENTLOOP_HPP__

#include "Epoll.hpp"
#include "IRun.hpp"
#include <vector>

namespace hpsf
{
    class Channel;
    class Epoll;

    class EventLoop:IChannelCallBack
    {
        public:
            EventLoop();
            ~EventLoop();

            void loop();
            void update(Channel* Channel);

            virtual void handleRead();
            virtual void handleWrite();

            void queueLoop(IRun* pRun);

        private:
            void wakeUp();
            int createEventfd();
            void doPendingFunctors();
            bool quit_;
            Epoll* poller_;
            int eventfd_;
            Channel* wakeupChannel_;
            std::vector<IRun*> pendingFunctors_;
    };
} // namespace hpsf


#endif