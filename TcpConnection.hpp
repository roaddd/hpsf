#ifndef __TCPCONNECTION_CPP__
#define __TCPCONNECTION_CPP__

#include "IChannelCallBcak.hpp"
#include "Channel.hpp"
#include "IMuduoUser.hpp"
#include "Buffer.hpp"
#include "IRun.hpp"


#include <string>
#include <memory>


namespace hpsf
{
    class IMuduoUser;
    class TcpServer;
    class TcpConnection:public IChannelCallBack,public IRun0,public IRun1,public IRun2,public std::enable_shared_from_this<TcpConnection>
    {
        public:
            TcpConnection(EventLoop* loop,int sockfd);
            ~TcpConnection();

            virtual void handleRead();
            virtual void handleWrite();
            virtual void run();
            virtual void run(const std::string& str,void* param);
            virtual void run(const std::string& str,std::any& any);

            void setUser(IMuduoUser* pUser);
            void setTcpServer(TcpServer* pUser){ pServer_=pUser;}
            void connectEstablished();
            void send(const std::string& s);
            int getFd(){return sockfd_;}
            EventLoop* getLoop(){return loop_;}
        private:
            EventLoop* loop_;
            int sockfd_;
            std::unique_ptr<Channel> pChannel_; 
            IMuduoUser* pUser_;
            TcpServer* pServer_;
            Buffer inBuf_;
            Buffer outBuf_;
    };

    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
} // namespace hpsf


#endif