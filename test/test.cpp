#include "net/TcpServer.hpp"
#include <iostream>
#include "net/EchoServer.hpp"

class Test1:public hpsf::IRun0
{
    public:
        virtual void run()
        {
            std::cout<<data<<std::endl;
        }
        std::string data;
};

hpsf::EventLoop* loop;
int64_t timer_id;

class Test2:public hpsf::IRun0
{
    public:
        virtual void run()
        {
            loop->cancelTimer(timer_id);
        }
};

int main()
{
    //EchoServer
    loop=new hpsf::EventLoop();
    hpsf::EchoServer server(loop,5,3);
    server.start();
    Test1 test1;
    Test2 test2;
    test1.data="每2s定时器";
    timer_id=loop->runEvery(2.0,&test1);
    loop->runAfter(8.0,&test2);
    loop->loop();
    return 0;
}