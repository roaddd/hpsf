#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "IChannelCallBcak.hpp"


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
        void setIndex(int index);
        int getfd();
        int getIndex() const;
        int revents() const;
        int getEvents() const;
        void enableReading();
        void disableReading();
        void enableWriting();
        void disableWriting();
        bool isWriting();
    private:
        void update();
        EventLoop* loop_;
        int sockfd_;
        int events_,revents_;
        int index_;
        IChannelCallBack* callBack_;
    };    
    
} // namespace hpsf


#endif