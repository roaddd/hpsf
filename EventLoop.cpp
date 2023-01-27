#include "EventLoop.hpp"
#include <vector>

namespace hpsf
{
    EventLoop::EventLoop():quit_(false)
    {
        poller_=new Epoll();
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
        }
        
    }

    void EventLoop::update(Channel* channel)
    {
        poller_->update(channel);
    }
} // namespace hpsf
