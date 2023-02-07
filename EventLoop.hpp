#ifndef __EVENTLOOP_HPP__
#define __EVENTLOOP_HPP__

#include "Epoll.hpp"
#include "IRun.hpp"
#include "TimerQueue.hpp"
#include "Mutex.hpp"
#include "TimerId.hpp"

#include <vector>
#include <memory>

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

            int64_t runAt(Timestamp when,IRun0* pRun);
            int64_t runAfter(double delay,IRun0* pRun);
            int64_t runEvery(double interval,IRun0* pRun);
            void cancelTimer(int64_t timerfd);

            bool isInLoopThread();

        private:
            void wakeUp();
            int createEventfd();
            void doPendingFunctors();
            bool quit_;
            bool callingPendingFunctors_;
            std::unique_ptr<Epoll> poller_;
            int eventfd_;
            const pid_t threadId_;
            std::unique_ptr<Channel> wakeupChannel_;
            MutexLock mutex_;
            std::vector<Task> pendingFunctors_;
            std::unique_ptr<TimerQueue> pTimerQueue_;
    };
} // namespace hpsf


#endif