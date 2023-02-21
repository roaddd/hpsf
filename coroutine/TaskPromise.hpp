#ifndef __TASKPROMISE_HPP__
#define __TASKPROMISE_HPP__

#include <coroutine> 

#include "TaskAwaiter.hpp"

template<typename T>
class Task;

template<typename T>
class TaskPromise
{
    public:
        Task<T> get_return_object(){return Task<T>{std::coroutine_handle<TaskPromise>::from_promise(*this)};}
        std::suspend_never initial_suspend(){return {};}
        std::suspend_always final_suspend() noexcept {return {};}
        void return_void(){}
        void unhandled_exception(){}

        // auto await_transform(Task<T>&& task)
        // {
        //     return TaskAwaiter<T>(std::move(task));
        // }
    //private:
};
    



#endif