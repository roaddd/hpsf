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
            :cond_(mutex_),
            running_(true)
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
                while(queue_.empty() && running_)
                {
                    cond_.wait();
                }
                if(!running_)return T();
                T front(queue_.front());
                queue_.pop_front();
                return front;
            }

            void stop()
            {
                running_=false;
            }
        private:
            //TODO:设置任务最大容量
            std::deque<T> queue_;
            MutexLock mutex_;
            Condition cond_;
            bool running_;
    };
} // namespace hpsf


#endif