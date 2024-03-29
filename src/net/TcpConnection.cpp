#include "net/TcpConnection.hpp"
#include "base/Define.hpp"
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>
#include <iostream>
#include "net/EventLoop.hpp"
#include "base/Task.hpp"
#include "net/TcpServer.hpp"

namespace hpsf
{
    TcpConnection::TcpConnection(EventLoop* loop,int sockfd):
        loop_(loop),
        sockfd_(sockfd),
        pUser_(nullptr),
        pChannel_(new Channel(loop_,sockfd_))
        { 
            pChannel_->setCallBack(this);
        }

    TcpConnection::~TcpConnection()
    {
        ::close(sockfd_);
    }

    void TcpConnection::setUser(IMuduoUser* pUser)
    {
        pUser_=pUser;
    }

    void TcpConnection::connectEstablished()
    {
        pChannel_->enableReading();
        if(pUser_)
            pUser_->onConnection(this);
    }

    void TcpConnection::handleRead()
    {
        char message[MAX_LINE];
        ssize_t readlength;
        bzero(message,MAX_LINE);
        int sockfd=pChannel_->getfd();
        if((readlength=read(sockfd,message,MAX_LINE))<0)
        {
            if(errno==ECONNRESET)
            {
                std::cout<<"Receive ECONNRESET from connfd: "<<sockfd<<", connfd closed"<<std::endl;
                close(sockfd);
            }
        }
        else if(readlength==0)
        {
            pChannel_->disableReading();
            pChannel_->disableWriting();
            TcpConnectionPtr guardThis(shared_from_this());
            pServer_->removeConnection(guardThis);
        }
        else
        {
            std::string buf(message,readlength);
            std::cout<<"handleRead(): "<<buf<<std::endl;
            inBuf_.append(buf);
            pUser_->onMessage(this,&inBuf_);
        }
    }

    void TcpConnection::handleWrite()
    {
        int sockfd=pChannel_->getfd();
        if(pChannel_->isWriting())
        {
            int n=::write(sockfd,outBuf_.peek(),outBuf_.readableBytes());
            if(n>0)
            {
                std::cout<<"write "<<n<<" bytes data again to "<<sockfd<<std::endl;
                outBuf_.retrive(n);
                if(outBuf_.readableBytes()==0)
                {
                    pChannel_->disableWriting();
                    Task task(this);
                    loop_->queueInLoop(task); //invoke onWriteComplate
                }
            }
        }
    }

    void TcpConnection::run()
    {
        pUser_->onWriteComplate(this);
    }

    void TcpConnection::run(const std::string& str,void* param)
    {
        if(str=="connectEstablished")connectEstablished();
    }

    void TcpConnection::run(const std::string& str,std::any& any)
    {
        if(str=="connectDestroyed")
        {
            TcpConnectionPtr conn=std::any_cast<TcpConnectionPtr>(any);
        }
    }

    void TcpConnection::send(const std::string& s)
    {
        int n=0;
        if(outBuf_.readableBytes()==0)
        {
            n=write(sockfd_,s.c_str(),s.size());
            if(n<0)
            {
                std::cout<<"TcpConnection::send error"<<std::endl;
                exit(1);
            }
            std::cout<<"n: "<<n<<" s.size(): "<<static_cast<int>(s.size())<<std::endl;
            if(n==static_cast<int>(s.size()))
            {
                Task task(this);
                loop_->queueInLoop(task);
            }
        }
        if(n<static_cast<int>(s.size()))
        {
            outBuf_.append(s.substr(n));
            if(!pChannel_->isWriting())
            {
                pChannel_->enableWriting();
            }
        }
    }
} // namespace hpsf
