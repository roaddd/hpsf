#include "co/Task.hpp"
#include "co/io_context.hpp"
#include "utils.hpp"

#include <coroutine>
#include <unistd.h>
#include <iostream>

using namespace std;
using namespace co;

Task<void> test()
{
    sleep(3);
    debug("run test() after 3s\n");    
    co_await TaskAwaiter<void>{};
} 

int main()
{
    io_context context;
    context.co_spawn(test());
    context.run();
    context.join();
}