#ifndef __IACCEPTORCALLBACK_HPP__
#define __IACCEPTORCALLBACK_HPP__

namespace hpsf
{
    class IAcceptorCallBack
    {
        public:
            virtual void newConnection(int connfd)
            { }
    };
} // namespace hpsf


#endif