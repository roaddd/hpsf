#ifndef __ACCEPTOR_HPP__
#define __ACCEPTOR_HPP__

#include "Channel.hpp"
#include "IAcceptorCallBack.hpp"
#include "IChannelCallBcak.hpp"

namespace hpsf
{
    class Acceptor:IChannelCallBack
    {
        public:
            Acceptor(int epollfd);
            ~Acceptor();

            virtual void OnIn(int socket);
            void setCallBack(IAcceptorCallBack* pCallBack);
            void start();

        private:
            int createAndListen();
            int epollfd_;
            int listenfd_;
            Channel* pAcceptChannel_;
            IAcceptorCallBack* pCallback_;
    };
} // namespace hpsf


#endif