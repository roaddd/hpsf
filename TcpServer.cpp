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
    loop_(loop),acceptor_(nullptr)  
    { 
    }

    TcpServer::~TcpServer()
    { }

    void TcpServer::start()
    {
        std::cout<<"TcpServer::start(): "<<std::endl;
        acceptor_=new Acceptor(loop_);
        acceptor_->setCallBack(this);
        acceptor_->start();
    }

    void TcpServer::setCallBack(IMuduoUser* pUser)
    {
        pUser_=pUser;
    }

    void TcpServer::newConnection(int connfd)
    {
        TcpConnection* connection=new TcpConnection(loop_,connfd);
        tcpConnections_[connfd]=connection;
        connection->setUser(pUser_);
        connection->connectEstablished();
    }

} // namespace hpsf

