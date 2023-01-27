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

    void TcpConnection::OnIn(int sockfd)
    {
        std::cout<<"OnIn: "<<sockfd<<std::endl;
        char message[MAX_LINE];
        ssize_t readlength;
        if(pChannel_->revents() & EPOLLIN)
        {
            bzero(message,MAX_LINE);
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
                pUser_->onMessage(this,buf);
            }
        }
    }

    void TcpConnection::send(std::string& s)
    {
        int n=::write(sockfd_,s.c_str(),s.size());
        if(n!=static_cast<int>(s.size()))
        {
            std::cout<<"TcpConnection::send error !"<<std::endl;
            exit(1);
        }
    }
} // namespace hpsf
