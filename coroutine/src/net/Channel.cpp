#include "../../include/net/Channel.hpp"

#include <sys/epoll.h>

namespace co
{
    Channel::Channel() : handle_(std::noop_coroutine()),
                         fd_(-1),
                         index_(0)
    {
    }

    Channel::Channel(std::coroutine_handle<> handle, int fd) : handle_(handle),
                                                               fd_(fd),
                                                               index_(0)
    {
    }

    Channel::~Channel()
    {
    }

    int Channel::revents() const
    {
        return revents_;
    }

    int Channel::getEvents() const
    {
        return events_;
    }

    void Channel::handleEvent()
    {
    }

    void Channel::setIndex(int index)
    {
        index_ = index;
    }

    int Channel::getfd()
    {
        return fd_;
    }

    int Channel::getIndex() const
    {
        return index_;
    }

    void Channel::enableReading()
    {

        events_ |= EPOLLIN;
        update();
    }

    void Channel::disableReading()
    {
        events_ &= ~EPOLLIN;
        update();
    }

    void Channel::enableWriting()
    {

        events_ |= EPOLLOUT;
        update();
    }

    void Channel::disableWriting()
    {
        events_ &= ~EPOLLOUT;
        update();
    }

    void Channel::update()
    {
        // co->update(this);
    }

    bool Channel::isWriting()
    {
        return events_ & EPOLLOUT;
    }
} // namespace co
