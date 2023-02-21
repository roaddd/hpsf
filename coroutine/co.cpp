#include <coroutine>
#include <unistd.h>
#include "Task.hpp"
#include <iostream>

Task<int> test()
{
    sleep(3);
    std::cout<<"test"<<std::endl;
    co_await TaskAwaiter<int>{};
} 

int main()
{
    test();
}