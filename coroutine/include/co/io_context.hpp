#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include <thread>
#include <queue>

#include "../../../Mutex.hpp"
#include "../../../Condition.hpp"

namespace co
{
    template<typename T>
    class Task;

    class io_context final
    {
        public:
            io_context();
            void co_spawn(Task<void>&& task);
            void join();
            void run();
        private:
            std::thread thread_;
            pid_t tid;
            std::queue<Task<void>> queue_;
            hpsf::MutexLock mutex_;
            std::atomic_bool is_active;
            hpsf::Condition queue_condition;
            //TODO:id
    };
} // namespace co


#endif