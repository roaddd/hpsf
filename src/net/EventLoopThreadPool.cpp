#include "net/EventLoopThreadPool.hpp"
#include "net/EventLoopThread.hpp"

namespace hpsf
{
    EventLoopThreadPool::EventLoopThreadPool(EventLoop* loop)
    :baseLoop_(loop),
    numThreads_(0),
    next_(0)
    {

    }

    EventLoopThreadPool::~EventLoopThreadPool()
    {

    }

    void EventLoopThreadPool::start()
    {
        for(int i=0;i<numThreads_;i++)
        {
            EventLoopThread* t=new EventLoopThread();
            threads_.push_back(std::unique_ptr<EventLoopThread>(t));
            loops_.push_back(t->startLoop());
        }
    }

    EventLoop* EventLoopThreadPool::getNextLoop()
    {
        EventLoop* loop=baseLoop_;
        if(!loops_.empty())
        {
            loop=loops_[next_];
            next_=(next_+1)%numThreads_;
        }
        return loop;
    }
} // namespace hpsf
