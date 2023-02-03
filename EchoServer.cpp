#include "EchoServer.hpp"
#include "Task.hpp"
#include "CurrentThread.hpp"

#include <iostream>

namespace hpsf
{
    EchoServer::EchoServer(EventLoop* loop,int numThreads):
    loop_(loop),
    tcpServer(loop),
    numThreads_(numThreads)
    {
        tcpServer.setCallBack(this);
        threadPool_.start(numThreads_);
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
        std::cout<<"send: "<<msg<<std::endl;
        Task task(this,msg,connection);
        threadPool_.addTask(task);
        std::cout<<"echo "<<msg.size() <<" bytes "<<std::endl;
    }

    void EchoServer::onWriteComplate(TcpConnection* connection)
    {
        std::cout<<"write complate to"<<std::endl;
    }

    //run in different thread
    void EchoServer::run(const std::string& str,void* param)
    {
        std::cout<<"fib(30)= "<<fib(30)<<" tid= "<<CurrentThread::tid()<<std::endl;
        TcpConnection* connection=static_cast<TcpConnection*>(param);
        connection->send(str);
    }

    int EchoServer::fib(int x)
    {
        return (x==1 || x==2)?1:(fib(x-1)+fib(x-2));
    }
}