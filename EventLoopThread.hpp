#ifndef __EVENTLOOPTHREAD_HPP__
#define __EVENTLOOPTHREAD_HPP__

#include "Thread.hpp"
#include "Mutex.hpp"
#include "Condition.hpp"

namespace hpsf
{
    class EventLoop;

    class EventLoopThread:public IRun0
    {
        public:
            EventLoopThread();
            ~EventLoopThread();
            EventLoop* startLoop();

            virtual void run();

        private:
            void threadFunc();
            EventLoop* loop_;
            Thread thread_;
            MutexLock mutex_;
            Condition cond_;
    };
} // namespace hpsf


#endif