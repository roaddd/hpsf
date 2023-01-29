#include "TcpConnection.hpp"
#include "Define.hpp"
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>
#include <iostream>

namespace hpsf
{
    TcpConnection::TcpConnection(EventLoop* loop,int sockfd):
        loop_(loop),sockfd_(sockfd),pUser_(nullptr)
        { 
            pChannel_=new Channel(loop_,sockfd_);
            pChannel_->setCallBack(this);
            pChannel_->enableReading();
        }

    TcpConnection::~TcpConnection()
    {

    }

    void TcpConnection::setUser(IMuduoUser* pUser)
    {
        pUser_=pUser;
    }

    void TcpConnection::connectEstablished()
    {
        if(pUser_)
            pUser_->onConnection(this);
    }

    void TcpConnection::handleRead()
    {
        char message[MAX_LINE];
        ssize_t readlength;
        bzero(message,MAX_LINE);
        int sockfd=pChannel_->getSockfd();
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
            std::cout<<"Read length 0 fron connfd: "<<sockfd<<", connfd closed"<<std::endl;
            close(sockfd);
        }
        else
        {
            std::string buf(message);
            inBuf_.append(buf);
            pUser_->onMessage(this,&inBuf_);
        }
    }

    void TcpConnection::handleWrite()
    {
        int sockfd=pChannel_->getSockfd();
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
                    loop_->queueLoop(this); //invoke onWriteComplate
                }
            }
        }
    }

    void TcpConnection::run()
    {
        pUser_->onWriteComplate(this);
    }

    void TcpConnection::send(std::string& s)
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
            if(n==static_cast<int>(s.size()))
            {
                loop_->queueLoop(this);
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
