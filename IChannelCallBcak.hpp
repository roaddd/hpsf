#ifndef __ICHANNELCALLBACK_HPP__
#define __ICHANNELCALLBACK_HPP__

namespace hpsf
{
    class IChannelCallBack
    {
    public:
        virtual void OnIn(int sockfd)
        { }

    };  
} // namespace hpsf



#endif