#include "TcpServer.hpp"
#include <iostream>
#include "EchoServer.hpp"

int main()
{
    hpsf::EventLoop* loop=new hpsf::EventLoop();
    hpsf::EchoServer server(loop);
    server.start();
    loop->loop();
    return 0;
}