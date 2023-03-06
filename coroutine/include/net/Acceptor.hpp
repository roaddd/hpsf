#ifndef __ACCEPTOR_HPP__
#define __ACCEPTOR_HPP__

#include "co/Task.hpp"
#include "Connection.hpp"
#include "co/io_context.hpp"

#include <memory>
#include <coroutine>
#include <utility>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

namespace co
{
    using promise_type = TaskPromise<int>;

    static __thread std::coroutine_handle<> handle_acceptor=nullptr;

    struct acceptor_awaiter
    {
        io_context *context_;
        int listenfd_;
        
        explicit acceptor_awaiter(io_context *context, int listenfd)
            : context_(context),
              listenfd_(listenfd)
        {
        }

        bool await_ready() const { return false; }

        /**
         * @brief
         *
         * @param awaiting_coro
         * @return true
         * @return false
         */
        bool await_suspend(std::coroutine_handle<> current) noexcept
        {
            if (handle_acceptor == nullptr)
            {
                handle_acceptor = current;
                //debug("context_->update() will run");
                context_->update(handle_acceptor, listenfd_);
            }
            return true;//返回当前协程的调用方/恢复方
        }

        auto await_resume() noexcept
        {
            int connfd;
            struct sockaddr_in cliaddr;
            socklen_t clilen = sizeof(cliaddr);
            //debug("will accept4\n");
            connfd = accept4(listenfd_, static_cast<sockaddr *>(static_cast<void *>(&cliaddr)), &clilen, SOCK_NONBLOCK);
            //debug("end accept4\n");
            return connfd;
        }
    };

    class Acceptor
    {
        friend struct acceptor_awaiter;

    public:
        Acceptor(io_context *context);
        ~Acceptor();

        void start();

        [[nondiscard]] acceptor_awaiter accept()
        {
            return acceptor_awaiter{context_,listenfd_};
        }

    private:
        int createAndListen();
        int listenfd_;
        io_context *context_;
    };
} // namespace hpsf

#endif