#ifndef __TCPSERVER_HPP__
#define __TCPSERVER_HPP__

#include "IChannelCallBcak.hpp"
#include "IAcceptorCallBack.hpp"
#include "Channel.hpp"
#include "Acceptor.hpp"
#include "TcpConnection.hpp"
#include "Epoll.hpp"
#include "EventLoop.hpp"
#include "IMuduoUser.hpp"

#include <map>
#include <memory>
#include <any>

namespace hpsf
{
    class EventLoopThreadPool;
    class TcpServer:public IAcceptorCallBack,public IRun2
    {
        public:
            TcpServer(EventLoop* loop);
            ~TcpServer();
            void start();
            virtual void newConnection(int connfd);
            void setCallBack(IMuduoUser* pUser);

            void setThreadNums(int num);

            void removeConnection(TcpConnectionPtr& conn);
            void removeConnectionInLoop(TcpConnectionPtr& conn);

            virtual void run(const std::string& str,std::any& any);
        private:
            EventLoop* loop_;
            std::unique_ptr<Acceptor> acceptor_;
            std::map<int,TcpConnectionPtr> tcpConnections_;
            IMuduoUser* pUser_;
            std::unique_ptr<EventLoopThreadPool> threadPool_;
    };
} // namespace hpsf



#endif
