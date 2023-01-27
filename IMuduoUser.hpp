#ifndef __IMUDUOUSER_HPP__
#define __IMUDUOUSER_HPP__

#include "TcpConnection.hpp"
#include <string>

namespace hpsf
{
    class TcpConnection;

    class IMuduoUser
    {
        public:
            virtual void onConnection(TcpConnection* connection)=0;
            virtual void onMessage(TcpConnection* connection,std::string& s)=0;

    };
} // namespace hpsf


#endif