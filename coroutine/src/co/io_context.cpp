#include "co/Task.hpp"
#include "co/io_context.hpp"
#include "utils.hpp"

namespace co
{
    io_context::io_context()
    :tid(-1),
    is_active(true),
    queue_condition(mutex_)
    { }

    void io_context::co_spawn(Task<void>&& task)
    {
        hpsf::MutexLockGuard guard(mutex_);
        queue_.emplace(std::move(task));
        queue_condition.notify();
    }

    void io_context::join()
    {
        thread_.join();
    }

    void io_context::run()
    {
        while(is_active.load(std::memory_order_relaxed) || !queue_.empty())
        {
            {
                hpsf::MutexLockGuard guard(mutex_);
                while (queue_.empty())
                {
                    queue_condition.wait();
                }
            }
            auto task{std::move(queue_.front())};
            queue_.pop();
            task.resume();
        }
        debug("loop thread exited\n");
    }
} // namespace co

