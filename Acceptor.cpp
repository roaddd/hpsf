#include "Acceptor.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

namespace hpsf
{
    Acceptor::Acceptor(EventLoop* loop):loop_(loop)
    ,listenfd_(-1),pAcceptChannel_(nullptr),pCallback_(nullptr)
    { 
        listenfd_=createAndListen();
    }

    Acceptor::~Acceptor()
    {
        close(listenfd_);

    }

    void Acceptor::setCallBack(IAcceptorCallBack* pCallBack)
    {
        pCallback_=pCallBack;
    }

    int Acceptor::createAndListen()
    {
        int listenfd=socket(AF_INET,SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK,0);
        
        //在close之后会有一个WAIT_TIME，导致bind失败的问题
        int val=1;
        if(setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,static_cast<void*>(&val),sizeof(int))==-1)
        {
            std::cout<<"setsockopt error"<<std::endl;
            exit(1);
        }
        
        struct sockaddr_in servaddr;
        int socklen=sizeof(servaddr);
        servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
        servaddr.sin_family=AF_INET;
        servaddr.sin_port=htons(8888);
        if(bind(listenfd,static_cast<sockaddr*>(static_cast<void*>(&servaddr)),static_cast<socklen_t>(socklen))==-1)
        {
            std::cout<<"bind error"<<std::endl;
        }
        if(listen(listenfd,SOMAXCONN))
        {
            std::cout<<"listen error"<<std::endl;
        }
        return listenfd;
    }

    void Acceptor::OnIn(int socket)
    {
        int connfd;
        struct sockaddr_in cliaddr;
        socklen_t clilen=sizeof(cliaddr);
        connfd=accept4(listenfd_,static_cast<sockaddr*>(static_cast<void*>(&cliaddr)),&clilen,SOCK_CLOEXEC|SOCK_NONBLOCK);
        if(connfd>0)
        {
            std::cout<<"new connection from "<<"["<<inet_ntoa(cliaddr.sin_addr)
            <<" "<<ntohs(cliaddr.sin_port)<<"]"<<" new socket fd: "<<connfd<<std::endl;
        }
        else
        {
            std::cout << "accept error, connfd:" << connfd
            << " errno:" << errno << std::endl;
        }
        pCallback_->newConnection(connfd);
    }

    void Acceptor::start()
    {
        std::cout<<"Acceptor::start(): "<<std::endl;
        pAcceptChannel_=new Channel(loop_,listenfd_);
        pAcceptChannel_->setCallBack(this);
        pAcceptChannel_->enableReading();
    }
} // namespace hpsf
