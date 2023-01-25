#ifndef __CHANNEL_HPP__
#define __CHANNEL_HPP__

#include "IChannelCallBcak.hpp"

namespace hpsf
{

    class Channel
    {
    public:
        Channel(int epollfd,int sockfd);
        ~Channel();
        void setCallBack(IChannelCallBack* callback);
        void handleEvent();
        void setRevents(int revent);
        int getSockfd();
        int revents() const;
        void enableReading();
    private:
        void update();
        int epollfd_;
        int sockfd_;
        int events_,revents_;
        IChannelCallBack* callBack_;
    };    
    
} // namespace hpsf


#endif