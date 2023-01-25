#include "TcpServer.hpp"

int main()
{
    hpsf::TcpServer server=hpsf::TcpServer();
    server.start();
    return 0;
}