#include "TcpServer.hpp"
#include <sys/epoll.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>

namespace hpsf
{
    TcpServer::TcpServer():epollfd_(-1),listenfd_(-1)
    { }

    TcpServer::~TcpServer()
    { }

    void TcpServer::start()
    {
        struct epoll_event events[MAX_EVENTS];
        
        int nfds;
        
        epollfd_=epoll_create1(0);
        if(epollfd_<0)
        {
            std::cout<<"epoll_create error, error: "<<epollfd_<<std::endl;
            exit(EXIT_FAILURE);
        }

        acceptor_=new Acceptor(epollfd_);
        acceptor_->setCallBack(this);
        acceptor_->start();
        

        for(;;)
        {
            std::vector<Channel*> activeChannels;
            nfds=epoll_wait(epollfd_,events,MAX_EVENTS,-1);
            if(nfds==-1)
            {
                std::cout<<"epoll_wait error, error: "<<errno<<std::endl;
                exit(EXIT_FAILURE);
            }
            for(int i=0;i<nfds;i++)
            {
                Channel* pChannel=static_cast<Channel*>(events[i].data.ptr);
                pChannel->setRevents(events[i].events);
                activeChannels.push_back(pChannel);
            }
            for(auto it=activeChannels.begin();it!=activeChannels.end();it++)
            {
                (*it)->handleEvent();
            }
        }
    }

    void TcpServer::newConnection(int connfd)
    {
        TcpConnection* connection=new TcpConnection(epollfd_,connfd);
        tcpConnections_[connfd]=connection;
    }

} // namespace hpsf

