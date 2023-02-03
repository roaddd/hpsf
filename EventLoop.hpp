#ifndef __EVENTLOOP_HPP__
#define __EVENTLOOP_HPP__

#include "Epoll.hpp"
#include "IRun.hpp"
#include <vector>
#include "TimerQueue.hpp"
#include "Mutex.hpp"

namespace hpsf
{
    class Channel;
    class Epoll;
    class Task;

    class EventLoop:IChannelCallBack
    {
        public:
            EventLoop();
            ~EventLoop();

            void loop();
            void update(Channel* Channel);

            virtual void handleRead();
            virtual void handleWrite();

            void queueInLoop(Task& task);
            void runInLoop(Task& task);

            int runAt(Timestamp when,IRun0* pRun);
            int runAfter(double delay,IRun0* pRun);
            int runEvery(double interval,IRun0* pRun);
            void cancelTimer(int timerfd);

            bool isInLoopThread();

        private:
            void wakeUp();
            int createEventfd();
            void doPendingFunctors();
            bool quit_;
            bool callingPendingFunctors_;
            Epoll* poller_;
            int eventfd_;
            const pid_t threadId_;
            Channel* wakeupChannel_;
            MutexLock mutex_;
            std::vector<Task> pendingFunctors_;
            TimerQueue* pTimerQueue_;
    };
} // namespace hpsf


#endif