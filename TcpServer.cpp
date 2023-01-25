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
    TcpServer::TcpServer():epollfd_(-1),listenfd_(-1)
    { }

    TcpServer::~TcpServer()
    { }

    int TcpServer::createAndListen()
    {
        int listenfd=socket(AF_INET,SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK,0);
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

    void TcpServer::OnIn(int sockfd)
    {
        std::cout<<"OnIn: "<<sockfd<<std::endl;
        if(sockfd==listenfd_)
        {
            struct sockaddr_in client;
            socklen_t solen=sizeof(client);
            struct epoll_event ev;
            int connfd=accept4(listenfd_,static_cast<struct sockaddr*>(static_cast<void*>(&client)),static_cast<socklen_t*>(static_cast<void*>(&solen)),
            SOCK_NONBLOCK | SOCK_CLOEXEC);
            if(connfd==-1)
            {
                std::cout<<"accept error"<<std::endl;
                exit(EXIT_FAILURE);
            }
            else
            {
                std::cout<<"new connection from "
                        <<"["<<inet_ntoa(client.sin_addr)
                        <<":"<<ntohs(client.sin_port)<<"]"
                        <<" new socket fd: "<<connfd<<std::endl;
            }
            //TODO:memory leak!
            Channel* pChannel =new Channel(epollfd_,connfd);
            pChannel->setCallBack(this);
            pChannel->enableReading();
            channels_[connfd]=pChannel;
        }
        else
        {
            char message[MAX_LINE];
            ssize_t readlength;
            Channel* channel=channels_[sockfd];
            int revents=channel->revents(),sockfd=channel->getSockfd();
            if(revents & EPOLLIN)
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
    }

    void TcpServer::start()
    {
        struct epoll_event events[MAX_EVENTS];
        
        int nfds;
        
        listenfd_=createAndListen();
        epollfd_=epoll_create1(0);
        if(epollfd_<0)
        {
            std::cout<<"epoll_create error, error: "<<epollfd_<<std::endl;
            exit(EXIT_FAILURE);
        }
        Channel* pChannel=new Channel(epollfd_,listenfd_);
        pChannel->setCallBack(this);
        pChannel->enableReading();

        for(;;)
        {
            std::vector<Channel*> activeChannels;
            nfds=epoll_wait(epollfd_,events,MAX_EVENTS,-1);
            if(nfds==-1)
            {
                std::cout<<"epoll_wait error, error: "<<errno<<std::endl;
                exit(EXIT_FAILURE);
            }
            for(int i=0;i<nfds;i++)
            {
                Channel* pChannel=static_cast<Channel*>(events[i].data.ptr);
                pChannel->setRevents(events[i].events);
                activeChannels.push_back(pChannel);
            }
            for(auto it=activeChannels.begin();it!=activeChannels.end();it++)
            {
                (*it)->handleEvent();
            }
        }
    }


} // namespace hpsf

