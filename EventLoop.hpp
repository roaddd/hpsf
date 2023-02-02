#ifndef __EVENTLOOP_HPP__
#define __EVENTLOOP_HPP__

#include "Epoll.hpp"
#include "IRun.hpp"
#include <vector>
#include "TimerQueue.hpp"

namespace hpsf
{
    class Channel;
    class Epoll;

    class EventLoop:IChannelCallBack
    {
        public:
            EventLoop();
            ~EventLoop();

            class Runner
            {
                public:
                    Runner(IRun* r,void* p):pRun_(r),param_(p)
                    { }
                    void doRun()
                    {
                        pRun_->run(param_);
                    }
                private:
                    IRun* pRun_;
                    void* param_;
            };

            void loop();
            void update(Channel* Channel);

            virtual void handleRead();
            virtual void handleWrite();

            void queueLoop(IRun* pRun,void* arg);

            int runAt(Timestamp when,IRun* pRun);
            int runAfter(double delay,IRun* pRun);
            int runEvery(double interval,IRun* pRun);
            void cancelTimer(int timerfd);

        private:
            void wakeUp();
            int createEventfd();
            void doPendingFunctors();
            bool quit_;
            Epoll* poller_;
            int eventfd_;
            Channel* wakeupChannel_;
            std::vector<Runner> pendingFunctors_;
            TimerQueue* pTimerQueue_;
    };
} // namespace hpsf


#endif