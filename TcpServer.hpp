#ifndef __TCPSERVER_HPP__
#define __TCPSERVER_HPP__

#include "IChannelCallBcak.hpp"
#include "IAcceptorCallBack.hpp"
#include "Channel.hpp"
#include <map>
#include "Acceptor.hpp"
#include "TcpConnection.hpp"
#include "Epoll.hpp"
#include "EventLoop.hpp"
#include "IMuduoUser.hpp"

namespace hpsf
{
    class TcpServer:IAcceptorCallBack
    {
        public:
            TcpServer(EventLoop* loop);
            ~TcpServer();
            void start();
            virtual void newConnection(int connfd);
            void setCallBack(IMuduoUser* pUser);
        private:
            EventLoop* loop_;
            int listenfd_;
            std::unique_ptr<Acceptor> acceptor_;
            std::map<int,TcpConnectionPtr> tcpConnections_;
            IMuduoUser* pUser_;
    };
} // namespace hpsf



#endif
