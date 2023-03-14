#ifndef __CONTEXT_HPP__
#define __CONTEXT_HPP__

#include <thread>
#include <queue>
#include <memory>
#include <unordered_map>
#include <coroutine>

#include "../../../include/base/Mutex.hpp"
#include "../../../include/base/Condition.hpp"
#include "net/Epoll.hpp"
#include "net/Channel.hpp"

namespace co
{
    template<typename T>
    class Task;

    using namespace hpsf;
    using namespace std;

    class io_context final
    {
        public:
            io_context();
            void co_spawn(Task<void>&& task,int fd);
            void join();
            void start();
            void update(int fd);
            void update(std::coroutine_handle<> handle,int fd);
        private:
            void run();
            thread thread_;
            pid_t tid;
            queue<std::coroutine_handle<>> queue_;
            MutexLock mutex_;
            atomic_bool is_active;
            Condition queue_condition;
            unique_ptr<Epoll> epoll_;
            //fd->coroutine_handle
            unordered_map<int,Channel> channels_;
            //TODO:id
    };
} // namespace co


#endif