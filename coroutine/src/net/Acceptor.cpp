#include "net/Acceptor.hpp"
#include "Utils.hpp"
#include "co/io_context.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

namespace co
{
    Acceptor::Acceptor(io_context *context)
        : listenfd_(createAndListen()),
          context_(context)
    {
        //debug("Acceptor created\n");
    }

    Acceptor::~Acceptor()
    {
        close(listenfd_);
    }

    int Acceptor::createAndListen()
    {
        int listenfd = socket(AF_INET, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);

        // 在close之后会有一个WAIT_TIME，导致bind失败的问题
        int val = 1;
        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, static_cast<void *>(&val), sizeof(int)) == -1)
        {
            //std::cout << "setsockopt error" << std::endl;
            exit(1);
        }

        struct sockaddr_in servaddr;
        int socklen = sizeof(servaddr);
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(8888);
        if (bind(listenfd, static_cast<sockaddr *>(static_cast<void *>(&servaddr)), static_cast<socklen_t>(socklen)) == -1)
        {
            //std::cout << "bind error" << std::endl;
            exit(1);
        }

        return listenfd;
    }

    void Acceptor::start()
    {
        if (listen(listenfd_, SOMAXCONN))
        {
            //std::cout << "listen error" << std::endl;
        }
       // std::cout << "Acceptor::start(): " << std::endl;
    }

} // namespace hpsf
