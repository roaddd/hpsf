#include "TcpServer.hpp"
#include "EventLoopThreadPool.hpp"
#include "Task.hpp"

#include <sys/epoll.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <any>

namespace hpsf
{
    TcpServer::TcpServer(EventLoop* loop):
    loop_(loop),
    acceptor_(new Acceptor(loop_)),
    threadPool_(new EventLoopThreadPool(loop)) 
    { 
    }

    TcpServer::~TcpServer()
    {
        ::close(listenfd_);
    }

    void TcpServer::start()
    {
        threadPool_->start();
        acceptor_->setCallBack(this);
        //TODO:runInLoop
        acceptor_->start();
    }

    void TcpServer::setCallBack(IMuduoUser* pUser)
    {
        pUser_=pUser;
    }

    void TcpServer::newConnection(int connfd)
    {
        EventLoop* ioLoop=threadPool_->getNextLoop();
        TcpConnectionPtr connection=std::make_shared<TcpConnection>(ioLoop,connfd);
        tcpConnections_[connfd]=connection;
        connection->setUser(pUser_);
        connection->setTcpServer(this);
        Task task(connection.get(),"connectEstablished",NULL);
        ioLoop->runInLoop(task);
    }

    void TcpServer::setThreadNums(int num)
    {
        threadPool_->setThreadNum(num);
    }

    void TcpServer::run(const std::string& str,std::any& any)
    {
        if(str=="removeConnection")
        {
            TcpConnectionPtr conn=std::any_cast<TcpConnectionPtr>(any);
            removeConnectionInLoop(conn);
        }
    }

    void TcpServer::removeConnection(TcpConnectionPtr& conn)
    {
        std::any c=conn;
        Task task(this,"removeConnection",c);
        
        std::cout<<"TcpServer::removeConnection fd:["<<conn->getFd()<<"]"<<std::endl;
        
        loop_->runInLoop(task);
    }

    void TcpServer::removeConnectionInLoop(TcpConnectionPtr& conn)
    {
        size_t n=tcpConnections_.erase(conn->getFd());
        (void)n;
        assert(n==1);
        EventLoop* ioLoop=conn->getLoop();
        std::any any=conn;
        Task task(conn.get(),"connectDestroyed",any);
        ioLoop->runInLoop(task);
    }

} // namespace hpsf

