#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include "IRun.hpp"
#include "BlockingQueue.hpp"
#include "Thread.hpp"


#include <vector>
#include <memory>

namespace hpsf
{
    class Task;
    class ThreadPool:public IRun0
    {
        public:
            ThreadPool();
            ~ThreadPool();
            void start(int numThreads);
            void addTask(Task& pTask);

            virtual void run();
        private:
            void runInThread();
            std::vector<std::unique_ptr<Thread>> threads_;
            BlockingQueue<Task> tasks_;
    };
} // namespace hpsf


#endif