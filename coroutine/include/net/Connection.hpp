#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__

#include <sys/socket.h>
#include <arpa/inet.h>

namespace co
{
    class Connection{
        public:
            Connection(int fd,struct sockaddr_in&& addr):
                fd_(fd),
                addr_(addr_)
                { }
        private:
            int fd_;
            struct sockaddr_in addr_;
    };
} // namespace co


#endif