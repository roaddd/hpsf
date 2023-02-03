#ifndef __THREADPOOL_HPP__
#define __THREADPOOL_HPP__

#include "IRun.hpp"
#include "BlockingQueue.hpp"

#include <vector>

namespace hpsf
{
    class Thread;
    class Task;
    class ThreadPool:public IRun0
    {
        public:
            ThreadPool();
            void start(int numThreads);
            void addTask(Task& pTask);

            virtual void run();
        private:
            void runInThread();
            std::vector<Thread*> threads_;
            BlockingQueue<Task> tasks_;
    };
} // namespace hpsf


#endif