#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <coroutine>
#include <utility>

#include "TaskPromise.hpp"


template<typename T=void>
class Task
{
    public:
        using promise_type=TaskPromise<T>;
        explicit Task(std::coroutine_handle<promise_type> handle) noexcept:
            handle(handle){ }
        Task(Task&& task) noexcept:
            handle(std::exchange(task.handle,{}))
            { }
    private:
        std::coroutine_handle<promise_type> handle;
};  

template<>
class Task<void>
{
    public:
        using promise_type=TaskPromise<void>;
        explicit Task(std::coroutine_handle<promise_type> handle) noexcept:
            handle(handle){ }
        Task(Task&& task) noexcept:
            handle(std::exchange(task.handle,{}))
            { }
    private:
        std::coroutine_handle<promise_type> handle;
};

#endif