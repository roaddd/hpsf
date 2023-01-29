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

    void EchoServer::onMessage(TcpConnection* connection,Buffer* buffer)
    {   
        std::string msg(buffer->retrieveAllAsString());
        std::cout<<"send: "<<msg<<"  "<<msg[msg.size()-3]-0<<" "<<msg.back()-0<<std::endl;
        connection->send(msg);
        std::cout<<"echo "<<msg.size() <<" bytes "<<std::endl;
    }

    void EchoServer::onWriteComplate(TcpConnection* connection)
    {
        std::cout<<"write complate to"<<std::endl;
    }
}