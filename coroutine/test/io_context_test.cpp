#include "co/Task.hpp"
#include "co/io_context.hpp"
#include "net/Acceptor.hpp"
#include "Utils.hpp"
#include "net/Socket.hpp"

#include <coroutine>
#include <unistd.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace co;

io_context context;

Task<> session(int sockfd){
    char buf[8192];
    int pos_r=0,pos_s=0;
    Socket sock{&context,sockfd};
    while(true){
        int nr=co_await sock.receive(buf,pos_r,8192);
        co_await sock.send(buf,pos_s,pos_r);
    }
}

Task<> server(io_context* co){
    debug("run work()\n");
    Acceptor accept(co);
    accept.start();
    for(int sock;sock=co_await accept.accept();){
        //debug("new connextion come: fd= ",sock);
        context.co_spawn(session(sock),sock);
    }
} 

int main()
{
    context.co_spawn(server(&context),-1);
    context.start();
    context.join();
}