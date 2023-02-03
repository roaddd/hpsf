#include "ThreadPool.hpp"
#include "Thread.hpp"
#include "Task.hpp"
#include <iostream>

namespace hpsf
{
    ThreadPool::ThreadPool()
    { }

    void ThreadPool::start(int numThreads)
    {
        threads_.reserve(numThreads);
        for(int i=0;i<numThreads;i++)
        {
            Thread* p=new Thread(this);
            threads_.push_back(p);
            p->start();
        }
    }

    void ThreadPool::addTask(Task& pTask)
    {
        tasks_.put(pTask);
    }

    void ThreadPool::run()
    {
        runInThread();
    }

    void ThreadPool::runInThread()
    {
        std::cout<<"runInThread()"<<std::endl;
        while(true)
        {
            Task task=tasks_.take();
            task.doTask();
        }
    }
} // namespace hpsf
