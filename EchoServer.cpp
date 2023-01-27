#include "EchoServer.hpp"
#include <iostream>

namespace hpsf
{
    EchoServer::EchoServer(EventLoop* loop):
    loop_(loop),tcpServer(loop)
    {
        tcpServer.setCallBack(this);
    }

    EchoServer::~EchoServer()
    {

    }

    void EchoServer::start()
    {
        tcpServer.start();
    }

    void EchoServer::onConnection(TcpConnection* connection)
    {
        std::cout<<"EchoServer- "<<std::endl;
    }

    void EchoServer::onMessage(TcpConnection* connection,std::string& s)
    {   
        std::cout<<"onMessage"<<std::endl;
        connection->send(s);
    }
}