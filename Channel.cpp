#include "Channel.hpp"
#include <sys/epoll.h>
#include <iostream>

namespace hpsf
{
    Channel::Channel(EventLoop* loop,int sockfd):
    loop_(loop),sockfd_(sockfd)
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
            callBack_->OnIn(sockfd_);
    }
    
    void Channel::setRevents(int revent)
    {
        revents_=revent;
    }

    int Channel::getSockfd()
    {
        return sockfd_;
    }

    void Channel::enableReading()
    {

        events_|=EPOLLIN;
        update();
    }
    
    void Channel::update()
    {
        loop_->update(this); 
    }

} // namespace hpsf
