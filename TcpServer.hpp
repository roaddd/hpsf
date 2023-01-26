#ifndef __TCPSERVER_HPP__
#define __TCPSERVER_HPP__

#include "IChannelCallBcak.hpp"
#include "IAcceptorCallBack.hpp"
#include "Channel.hpp"
#include <map>
#include "Acceptor.hpp"
#include "TcpConnection.hpp"

#define MAX_LINE 100
#define MAX_EVENTS 500
#define MAX_LISTENFD 200

namespace hpsf
{
    class TcpServer:IAcceptorCallBack
    {
        public:
            TcpServer();
            ~TcpServer();
            void start();
            virtual void newConnection(int connfd);
        private:
            
            int epollfd_;
            int listenfd_;
            Acceptor* acceptor_;
            std::map<int,TcpConnection*> tcpConnections_;
    };
} // namespace hpsf



#endif
