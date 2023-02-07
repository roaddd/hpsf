#include "EventLoop.hpp"
#include "CurrentThread.hpp"
#include "Task.hpp"

#include <vector>
#include <sys/eventfd.h>
#include <iostream>
#include <unistd.h>

namespace hpsf
{
    EventLoop::EventLoop()
    :quit_(false),
    threadId_(CurrentThread::tid()),
    callingPendingFunctors_(false),
    poller_(new Epoll()),
    eventfd_(createEventfd()),
    wakeupChannel_(new Channel(this,eventfd_)),
    pTimerQueue_(new TimerQueue(this))
    {
        wakeupChannel_->setCallBack(this);
        wakeupChannel_->enableReading();
        std::cout<<"EventLoop created "<<this<<" in thread "<<threadId_<<std::endl;
    }

    EventLoop::~EventLoop()
    {
        std::cout<<"EventLoop "<<this<<" of thread "<<threadId_
                <<" destructs in thread "<<CurrentThread::tid()<<std::endl;
        //TODO:wakeupChannel

        ::close(eventfd_);
    }

    void EventLoop::loop()
    {
        std::cout << "EventLoop " << this << " start looping"<<"\n";
        while(!quit_)
        {
            std::vector<Channel*> activeChannels;
            poller_->poll(&activeChannels);
            for(auto it=activeChannels.begin();it!=activeChannels.end();it++)
            {
                (*it)->handleEvent();
            }

            doPendingFunctors();
        }
        std::cout << "EventLoop " << this << " stop looping"<<"\n";
    }

    void EventLoop::update(Channel* channel)
    {
        poller_->update(channel);
    }

    int EventLoop::createEventfd()
    {
        int evfd=::eventfd(0,EFD_NONBLOCK | EFD_CLOEXEC);
        if(evfd<0)
        {
            std::cout<<"Failed in EventLoop::createEventfd()"<<std::endl;
            exit(1);
        }
        return evfd;
    }

    void EventLoop::handleRead()
    {
        uint64_t one=1;
        ssize_t n=::read(eventfd_,&one,sizeof(one));
        if(n!=sizeof(one))
        {
            std::cout<<"EventLoop::handleRead()"<<std::endl;
        }
    }

    void EventLoop::handleWrite()
    {
        
    }

    void EventLoop::queueInLoop(Task& task)
    {
        {
            MutexLockGuard guard(mutex_);
            pendingFunctors_.emplace_back(task);
        }

        if(!isInLoopThread() || callingPendingFunctors_)
        {
            wakeUp();
        }
    }

    void EventLoop::runInLoop(Task& task)
    {
        if(isInLoopThread())
        {
            task.doTask();
        }
        else
        {
            queueInLoop(task);
        }
    }

    void EventLoop::wakeUp()
    {
        uint64_t one=1;
        ssize_t n=::write(eventfd_,&one,sizeof(one));
        if(n!=sizeof(one))
        {
            std::cout<<"EventLoop::wakeUp() writes "<<n<<" bytes instead of 8"<<std::endl;
            exit(1);
        }
    }

    void EventLoop::doPendingFunctors()
    {
        std::vector<Task> nextRuns;
        callingPendingFunctors_=true;
        {
            MutexLockGuard guard(mutex_);
            nextRuns.swap(pendingFunctors_);
        }
        for(auto it=nextRuns.begin();it!=nextRuns.end();it++)
        {
            it->doTask();
        }
        callingPendingFunctors_=false;
    }

    int64_t EventLoop::runAt(Timestamp when,IRun0* pRun)
    {
        return pTimerQueue_->addTimer(pRun,when,0.0);
    }

    int64_t EventLoop::runAfter(double delay,IRun0* pRun)
    {
        return pTimerQueue_->addTimer(pRun,Timestamp::nowAfter(delay),0.0);
    }

    int64_t EventLoop::runEvery(double interval,IRun0* pRun)
    {
        return pTimerQueue_->addTimer(pRun,Timestamp::nowAfter(interval),interval);
    }

    void EventLoop::cancelTimer(int64_t timerId)
    {
        pTimerQueue_->cancel(timerId); 
    }

    bool EventLoop::isInLoopThread()
    {
        return threadId_==CurrentThread::tid();
    }

} // namespace hpsf
