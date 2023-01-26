#ifndef __TCPCONNECTION_CPP__
#define __TCPCONNECTION_CPP__

#include "IChannelCallBcak.hpp"
#include "Channel.hpp"


namespace hpsf
{
    class TcpConnection:public IChannelCallBack
    {
        public:
            TcpConnection(int epollfd,int sockfd);
            ~TcpConnection();

            virtual void OnIn(int sockfd);
        private:
            int epollfd_;
            int sockfd_;
            Channel* pChannel_; 

    };
} // namespace hpsf


#endif