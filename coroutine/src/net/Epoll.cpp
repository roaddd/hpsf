#include "net/Epoll.hpp"
#include "Utils.hpp"

#include <assert.h>
#include <unistd.h>

namespace co
{
    enum State
    {
        kNew,
        kAdded
    };

    Epoll::Epoll()
    {
        epollfd_ = epoll_create1(EPOLL_CLOEXEC);
        assert(epollfd_>0);
    }

    Epoll::~Epoll()
    {
        close(epollfd_);
    }

    void Epoll::poll(std::vector<Channel *> *pChannels)
    {
        //debug("epoll start");
        int nfds = epoll_wait(epollfd_, events_, MAX_EVENTS, -1);
        //debug("epoll end: nfds=",nfds);
        assert(nfds>0);
        for (int i = 0; i < nfds; i++)
        {
            Channel *pChannel = static_cast<Channel *>(events_[i].data.ptr);
            pChannel->setRevents(events_[i].events);
            pChannels->push_back(pChannel);
        }
    }

    void Epoll::update(Channel *channel)
    {
        int index = channel->getIndex();
        struct epoll_event ev;
        ev.data.ptr = channel;
        ev.events = channel->getEvents();
        int fd = channel->getfd();
        //debug("Epoll::update(),fd= ",fd," will added");
        //debug("index= ",index);
        if (index == kNew)
        {
            channel->setIndex(kAdded);
            //debug("Epoll::update(),fd =",fd,"is added");
            ::epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev);
        }
        else
        {
            ::epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev);
        }
    }
} // namespace co
