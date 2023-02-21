#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <coroutine>
#include <utility>

#include "TaskPromise.hpp"

namespace co
{
    template<typename T>
    class Task
    {
        public:
            using promise_type=TaskPromise<T>;
            explicit Task(std::coroutine_handle<promise_type> handle) noexcept:
                handle(handle){ }
            constexpr Task(Task&& task) noexcept:
                handle(std::exchange(task.handle,{}))
                { }

            void resume(){handle.resume();}
        private:
            std::coroutine_handle<promise_type> handle;
    };  
    
} // namespace co

#endif