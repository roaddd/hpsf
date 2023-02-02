#include "TcpServer.hpp"
#include <iostream>
#include "EchoServer.hpp"

class Test1:public hpsf::IRun
{
    public:
        virtual void run(void* param)
        {
            std::cout<<data<<std::endl;
        }
        std::string data;
};

int main()
{
    // hpsf::EventLoop* loop=new hpsf::EventLoop();
    // hpsf::EchoServer server(loop);
    // server.start();
    // loop->loop();

    //TimerQueue
    hpsf::EventLoop* loop=new hpsf::EventLoop();
    Test1 test1;
    test1.data="每2s定时器";
    loop->runEvery(2.0,&test1);
    loop->loop();
    return 0;
}