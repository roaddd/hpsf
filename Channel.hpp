#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "IChannelCallBcak.hpp"
#include "EventLoop.hpp"

namespace hpsf
{
    class EventLoop;

    class Channel
    {
    public:
        Channel(EventLoop* loop,int sockfd);
        ~Channel();
        void setCallBack(IChannelCallBack* callback);
        void handleEvent();
        void setRevents(int revent);
        int getSockfd();
        int revents() const;
        int getEvents() const;
        void enableReading();
    private:
        void update();
        EventLoop* loop_;
        int sockfd_;
        int events_,revents_;
        IChannelCallBack* callBack_;
    };    
    
} // namespace hpsf


#endif