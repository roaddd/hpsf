#ifndef __BLOCKINGQUEUE_HPP__
#define __BLOCKINGQUEUE_HPP__

#include "Mutex.hpp"
#include "Condition.hpp"
#include <deque>


namespace hpsf
{
    template<typename T>
    class BlockingQueue
    {
        public:
            BlockingQueue()
            :cond_(mutex_)
            { }

            void put(const T& t)
            {
                MutexLockGuard lock(mutex_);
                queue_.emplace_back(t);
                cond_.notify();
            }

            T take()
            {
                MutexLockGuard lock(mutex_);
                while(queue_.empty())
                {
                    cond_.wait();
                }
                T front(queue_.front());
                queue_.pop_front();
                return front;
            }

        private:
            std::deque<T> queue_;
            MutexLock mutex_;
            Condition cond_;
    };
} // namespace hpsf


#endif