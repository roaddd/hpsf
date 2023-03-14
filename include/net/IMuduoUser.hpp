#ifndef __IMUDUOUSER_HPP__
#define __IMUDUOUSER_HPP__

#include "TcpConnection.hpp"
#include <string>
#include "../base/Buffer.hpp"

namespace hpsf
{
    class TcpConnection;

    class IMuduoUser
    {
        public:
            virtual void onConnection(TcpConnection* connection)=0;
            virtual void onMessage(TcpConnection* connection,Buffer* buffer)=0;
            virtual void onWriteComplate(TcpConnection* connection)=0;
    };
} // namespace hpsf


#endif