#include "TcpConnection.hpp"
#include "Define.hpp"
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>
#include <errno.h>
#include <iostream>

namespace hpsf
{
    TcpConnection::TcpConnection(int epollfd,int sockfd):
        epollfd_(epollfd),sockfd_(sockfd)
        { 
            pChannel_=new Channel(epollfd_,sockfd_);
            pChannel_->setCallBack(this);
            pChannel_->enableReading();
        }

    TcpConnection::~TcpConnection()
    {

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
                if(write(sockfd,message,readlength)<0)
                {
                    std::cout<<"can't write all message to connfd: "<<sockfd<<std::endl;
                }
            }
        }
    }
} // namespace hpsf
