#ifndef __EVENTLOOPTHREADPOOL_HPP__
#define __EVENTLOOPTHREADPOOL_HPP__

#include "EventLoopThread.hpp"

#include <vector>
#include <memory>


namespace  hpsf
{
    class EventLoop;
    class EventLoopThreadPool
    {
        public:
            EventLoopThreadPool(EventLoop* loop);
            ~EventLoopThreadPool();

            void setThreadNum(int num){numThreads_=num;}
            void start();

            EventLoop* getNextLoop();
        private:
            EventLoop* baseLoop_;
            int numThreads_;
            int next_;
            std::vector<std::unique_ptr<EventLoopThread>> threads_;
            std::vector<EventLoop*> loops_;
    };
} // namespace  hpsf


#endif