#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <coroutine>
#include <utility>

#include "Utils.hpp"

namespace co
{
    template <typename T>
    class Task;

    struct awaiter_base;

    template <typename T = void>
    class TaskPromise
    {
    public:
        Task<T> get_return_object()
        {
            handle = std::coroutine_handle<TaskPromise>::from_promise(*this);
            //debug("get_return_object() ", &handle);
            return Task<T>{handle};
        }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept
        {
            //debug("final_suspend() ", &handle, &parent_coro);
            return {};
        }
        void return_void() {}
        void unhandled_exception() {}

        void set_parent(std::coroutine_handle<> continuation) noexcept
        {
            //debug(&handle, " set_parent ", &continuation);
            parent_coro = continuation;
        }
        // auto await_transform(int val)
        // {
        //     struct awaiter : awaiter_base
        //     {
        //         using awaiter_base::awaiter_base;
        //     };
        //     this->result = val;
        //     return awaiter{handle};
        // }
        // private:
        std::coroutine_handle<TaskPromise> handle;
        // 记录caller
        std::coroutine_handle<> parent_coro{std::noop_coroutine()};
        int result;
    };

    template <typename T = void>
    class Task
    {
        friend class TaskPromise<T>;
        friend class Acceptor;

    public:
        using promise_type = TaskPromise<T>;
        using value_type = T;

        explicit Task(std::coroutine_handle<promise_type> handle) noexcept : handle(handle) {}
        constexpr Task(Task &&task) noexcept : handle(std::exchange(task.handle, {}))
        {
        }

        void resume() { handle.resume(); }

    private:
        struct awaiter_base
        {
            std::coroutine_handle<promise_type> handle;
            explicit awaiter_base(std::coroutine_handle<promise_type> current) noexcept
                : handle(current) {}
            bool await_ready() const { return false; }

            std::coroutine_handle<>
            await_suspend(std::coroutine_handle<> awaiting_coro) noexcept
            {
                // debug(&handle," await_suspend ",&awaiting_coro);
                handle.promise().set_parent(awaiting_coro);
                return handle;
            }
        };

    public:
        ~Task()
        {
            if (handle)
                handle.destroy();
        }

        auto operator co_await() const &noexcept
        {
            //debug("co_await() ", &handle);
            struct awaiter : awaiter_base
            {
                using awaiter_base::awaiter_base;

                decltype(auto) await_resume()
                {
                    return this->handle.promise().result;
                }
            };

            return awaiter{handle};
        }

        auto get_handle() noexcept { return handle; }

        void detach() noexcept{
            handle=nullptr;
        }

    private:
        std::coroutine_handle<promise_type> handle;
        // int fd_;
    };

} // namespace co

#endif