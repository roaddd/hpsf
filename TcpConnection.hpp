#ifndef __TCPCONNECTION_CPP__
#define __TCPCONNECTION_CPP__

#include "IChannelCallBcak.hpp"
#include "Channel.hpp"
#include "IMuduoUser.hpp"
#include "Buffer.hpp"
#include "IRun.hpp"

#include <string>

namespace hpsf
{
    class IMuduoUser;
    class TcpConnection:public IChannelCallBack,public IRun0
    {
        public:
            TcpConnection(EventLoop* loop,int sockfd);
            ~TcpConnection();

            virtual void handleRead();
            virtual void handleWrite();
            virtual void run();

            void setUser(IMuduoUser* pUser);
            void connectEstablished();
            void send(const std::string& s);
        private:
            EventLoop* loop_;
            int sockfd_;
            Channel* pChannel_; 
            IMuduoUser* pUser_;
            Buffer inBuf_;
            Buffer outBuf_;
    };
} // namespace hpsf


#endif