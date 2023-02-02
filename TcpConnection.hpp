#ifndef __TCPCONNECTION_CPP__
#define __TCPCONNECTION_CPP__

#include "IChannelCallBcak.hpp"
#include "Channel.hpp"
#include "IMuduoUser.hpp"
#include <string>
#include "Buffer.hpp"
#include "IRun.hpp"

namespace hpsf
{
    class IMuduoUser;
    class TcpConnection:public IChannelCallBack,public IRun
    {
        public:
            TcpConnection(EventLoop* loop,int sockfd);
            ~TcpConnection();

            virtual void handleRead();
            virtual void handleWrite();
            virtual void run(void* param);

            void setUser(IMuduoUser* pUser);
            void connectEstablished();
            void send(std::string& s);
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