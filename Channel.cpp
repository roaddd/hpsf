#include "Channel.hpp"
#include <sys/epoll.h>
#include <iostream>
#include "EventLoop.hpp"

namespace hpsf
{
    Channel::Channel(EventLoop* loop,int sockfd):
    loop_(loop),sockfd_(sockfd),index_(0)
    ,events_(0),revents_(0),callBack_(nullptr)
    { }

    Channel::~Channel()
    { }

    int Channel::revents() const
    {
        return revents_;
    }

    int Channel::getEvents() const
    {
        return events_;
    }

    void Channel::setCallBack(IChannelCallBack* callback)
    {
        callBack_=callback;
    }

    void Channel::handleEvent()
    {
        if(revents_ & EPOLLIN)
            callBack_->handleRead();
        if(revents_ & EPOLLOUT)
            callBack_->handleWrite();
    }
    
    void Channel::setRevents(int revent)
    {
        revents_=revent;
    }

    void Channel::setIndex(int index)
    {
        index_=index;
    }

    int Channel::getfd()
    {
        return sockfd_;
    }

    int Channel::getIndex() const 
    {
        return index_;
    }

    void Channel::enableReading()
    {

        events_|=EPOLLIN;
        update();
    }
    
    void Channel::disableReading()
    {
        events_&=~EPOLLIN;
        update();
    }

    void Channel::enableWriting()
    {

        events_|=EPOLLOUT;
        update();
    }
    
    void Channel::disableWriting()
    {
        events_&=~EPOLLOUT;
        update();
    }


    void Channel::update()
    {
        loop_->update(this); 
    }

    bool Channel::isWriting()
    {
        return events_&EPOLLOUT;
    }

} // namespace hpsf
