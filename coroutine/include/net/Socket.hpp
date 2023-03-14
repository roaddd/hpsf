#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

#include "Utils.hpp"

#include <unistd.h>
#include <coroutine>

namespace co
{
    /**
     * @brief receive()函数返回值
     * 
     */
    struct socket_receive_awaiter
    {
        int bytes_;

        explicit socket_receive_awaiter(int bytes) : bytes_(bytes)
        {
        }

        bool await_ready() const { return false; }

        bool await_suspend(std::coroutine_handle<> awaiting_coro) noexcept
        {
            // debug(&handle," await_suspend ",&awaiting_coro);
            return false;
        }

        int await_resume() noexcept
        {
            return bytes_;
        }
    };

    /**
     * @brief send()函数返回值
     * 
     */
    struct socket_send_awaiter
    {
        int bytes_;

        explicit socket_send_awaiter(int bytes) : bytes_(bytes)
        {
        }

        bool await_ready() const { return false; }

        bool await_suspend(std::coroutine_handle<> awaiting_coro) noexcept
        {
            // debug(&handle," await_suspend ",&awaiting_coro);
            return true;
        }

        int await_resume() noexcept
        {
            return bytes_;
        }
    };

    class Socket
    {
    public:
        Socket(io_context* context,int sockfd) noexcept 
        : sockfd_(sockfd),
        context_(context)
        {
        }

        Socket(Socket &&other)
        {
            sockfd_ = other.sockfd_;
            context_=other.context_;
            other.sockfd_ = -1;
            other.context_=nullptr;
        }

        Socket& operator=(Socket &&other)
        {
            sockfd_=other.sockfd_;
            context_=other.context_;
            other.sockfd_ = -1;
            other.context_=nullptr;
            return *this;
        }

        ~Socket() noexcept
        {
            close(sockfd_);
        }

        [[nodiscard]] auto receive(char *s, int &pos, int allBytes)
        {
            int n = read(sockfd_, static_cast<void *>(s + pos), allBytes - pos);
            if(n==0){
                close(sockfd_);
                context_->update(sockfd_);
            }
            pos += n;
            return socket_receive_awaiter{n};
        }

        [[nodiscard]] auto send(char *s, int &pos, int allBytes)
        {
            int n = write(sockfd_, static_cast<const void *>(s + pos), allBytes - pos);
            pos += n;
            return socket_send_awaiter{n};
        }

    private:
        int sockfd_;
        io_context* context_;
    };
} // namespace co

#endif