#include "net/EventLoopThread.hpp"
#include "net/EventLoop.hpp"

namespace hpsf
{
    EventLoopThread::EventLoopThread()
    :thread_(this),
    mutex_(),
    loop_(NULL),
    cond_(mutex_)
    {

    }

    EventLoopThread::~EventLoopThread()
    {
        loop_->quit();
        thread_.join();
    }

    EventLoop* EventLoopThread::startLoop()
    {
        thread_.start();
        EventLoop* loop=NULL;
        {
            MutexLockGuard lock(mutex_);
            while(loop_==NULL)
            {
                cond_.wait();
            }
            loop=loop_;
        }
        return loop;
    }

    void EventLoopThread::run()
    {
        threadFunc();
    }

    void EventLoopThread::threadFunc()
    {
        EventLoop loop;
        {
            MutexLockGuard lock(mutex_);
            loop_=&loop;
            cond_.notify();
        }
        loop_->loop();
    }
} // namespace hpsf
