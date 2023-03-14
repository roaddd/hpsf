#include "base/ThreadPool.hpp"
#include "base/Thread.hpp"
#include "base/Task.hpp"
#include <iostream>

namespace hpsf
{
    ThreadPool::ThreadPool()
    { }

    ThreadPool::~ThreadPool()
    {
        tasks_.stop();
        for(int i=0;i<threads_.size();i++)
        {
            threads_[i]->join();
        }
    }

    void ThreadPool::start(int numThreads)
    {
        threads_.reserve(numThreads);
        for(int i=0;i<numThreads;i++)
        {
            threads_.push_back(std::unique_ptr<Thread>(new Thread(this)));
            threads_[i]->start();
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
        while(true)
        {
            Task task=tasks_.take();
            if(task.valid())task.doTask();
        }
    }
} // namespace hpsf
