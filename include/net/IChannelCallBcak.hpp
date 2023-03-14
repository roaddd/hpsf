#ifndef __ICHANNELCALLBACK_HPP__
#define __ICHANNELCALLBACK_HPP__

namespace hpsf
{
    class IChannelCallBack
    {
    public:
        virtual void handleRead()=0;
        virtual void handleWrite()=0;
    };  
} // namespace hpsf



#endif