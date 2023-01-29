#include "EventLoop.hpp"
#include <vector>
#include <sys/eventfd.h>
#include <iostream>
#include <unistd.h>

namespace hpsf
{
    EventLoop::EventLoop():quit_(false)
    {
        poller_=new Epoll();
        eventfd_=createEventfd();
        wakeupChannel_=new Channel(this,eventfd_);
        wakeupChannel_->setCallBack(this);
        wakeupChannel_->enableReading();
    }

    EventLoop::~EventLoop()
    {

    }

    void EventLoop::loop()
    {
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

    void EventLoop::queueLoop(IRun* pRun)
    {
        pendingFunctors_.push_back(pRun);
        wakeUp();
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
        std::vector<IRun*> nextRuns;
        nextRuns.swap(pendingFunctors_);
        for(auto it=nextRuns.begin();it!=nextRuns.end();it++)
        {
            (*it)->run();
        }
    }

} // namespace hpsf
