#ifndef __ECHOSERVER_HPP__
#define __ECHOSERVER_HPP__

#include "IMuduoUser.hpp"
#include "TcpServer.hpp"

namespace hpsf
{
    
    class EchoServer:public IMuduoUser
    {
        public:
            EchoServer(EventLoop* loop);
            ~EchoServer();

            virtual void onConnection(TcpConnection* connection);
            virtual void onMessage(TcpConnection* connection,std::string& s);

            void start();
        private:
            EventLoop* loop_;
            TcpServer tcpServer;
    };
    
} // namespace hpsf


#endif