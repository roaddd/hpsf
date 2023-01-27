#ifndef __TCPCONNECTION_CPP__
#define __TCPCONNECTION_CPP__

#include "IChannelCallBcak.hpp"
#include "Channel.hpp"
#include "IMuduoUser.hpp"
#include <string>

namespace hpsf
{
    class IMuduoUser;
    class TcpConnection:public IChannelCallBack
    {
        public:
            TcpConnection(EventLoop* loop,int sockfd);
            ~TcpConnection();

            virtual void OnIn(int sockfd);

            void setUser(IMuduoUser* pUser);
            void connectEstablished();
            void send(std::string& s);
        private:
            EventLoop* loop_;
            int sockfd_;
            Channel* pChannel_; 
            IMuduoUser* pUser_;
    };
} // namespace hpsf


#endif