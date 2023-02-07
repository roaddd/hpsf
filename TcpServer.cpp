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
    TcpServer::TcpServer(EventLoop* loop):
    loop_(loop),acceptor_(new Acceptor(loop_))  
    { 
    }

    TcpServer::~TcpServer()
    {
        ::close(listenfd_);
    }

    void TcpServer::start()
    {
        std::cout<<"TcpServer::start(): "<<std::endl;
        acceptor_->setCallBack(this);
        //TODO 
        acceptor_->start();
    }

    void TcpServer::setCallBack(IMuduoUser* pUser)
    {
        pUser_=pUser;
    }

    void TcpServer::newConnection(int connfd)
    {
        TcpConnectionPtr connection=std::make_shared<TcpConnection>(loop_,connfd);
        tcpConnections_[connfd]=connection;
        connection->setUser(pUser_);
        connection->connectEstablished();
    }

} // namespace hpsf

