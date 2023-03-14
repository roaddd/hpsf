#include "net/Epoll.hpp"
#include <unistd.h>
#include <iostream>

namespace hpsf
{

    enum State
    {
        kNew,kAdded
    };

    Epoll::Epoll()
    {
        epollfd_=epoll_create1(EPOLL_CLOEXEC);
        if(epollfd_<=0)
        {
            std::cout<<"epoll_create1 error"<<std::endl;
            exit(EXIT_FAILURE);
        }
    }

    Epoll::~Epoll()
    {
        close(epollfd_);
    }

    void Epoll::poll(std::vector<Channel*>* pChannels)
    {
        int nfds=epoll_wait(epollfd_,events_,MAX_EVENTS,-1);
        if(nfds==-1)
        {
            std::cout<<"epoll_wait error, error: "<<errno<<std::endl;
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<nfds;i++)
        {
            Channel* pChannel=static_cast<Channel*>(events_[i].data.ptr);
            pChannel->setRevents(events_[i].events);
            pChannels->push_back(pChannel);
        }
    }

    

    void Epoll::update(Channel* channel)
    {
        int index=channel->getIndex();
        struct epoll_event ev;
        ev.data.ptr=channel;
        ev.events=channel->getEvents();
        int fd=channel->getfd();
        if(index==kNew)
        {
            channel->setIndex(kAdded);
            ::epoll_ctl(epollfd_,EPOLL_CTL_ADD,fd,&ev);
        }
        else
        {
            ::epoll_ctl(epollfd_,EPOLL_CTL_MOD,fd,&ev);
        }
    }
} // namespace hpsf
