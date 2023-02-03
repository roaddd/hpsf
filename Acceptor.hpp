#ifndef __ACCEPTOR_HPP__
#define __ACCEPTOR_HPP__

#include "Channel.hpp"
#include "IAcceptorCallBack.hpp"
#include "IChannelCallBcak.hpp"

namespace hpsf
{
    class Acceptor:public IChannelCallBack
    {
        public:
            Acceptor(EventLoop* loop);
            ~Acceptor();

            virtual void handleRead();
            virtual void handleWrite();
            void setCallBack(IAcceptorCallBack* pCallBack);
            void start();

        private:
            int createAndListen();
            EventLoop* loop_;
            int listenfd_;
            Channel* pAcceptChannel_;
            IAcceptorCallBack* pCallback_;
    };
} // namespace hpsf


#endif