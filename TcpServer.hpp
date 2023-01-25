#ifndef __TCPSERVER_HPP__
#define __TCPSERVER_HPP__

#include "IChannelCallBcak.hpp"
#include "Channel.hpp"
#include <map>

#define MAX_LINE 100
#define MAX_EVENTS 500
#define MAX_LISTENFD 200

namespace hpsf
{
    class TcpServer:IChannelCallBack
    {
        public:
            TcpServer();
            ~TcpServer();
            void start();
            virtual void OnIn(int sockfd);
        private:
            int createAndListen();
            int epollfd_;
            int listenfd_;
            std::map<int,Channel*> channels_;
    };
} // namespace hpsf



#endif
