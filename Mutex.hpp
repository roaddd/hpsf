#ifndef __MUTEX_HPP__
#define __MUTEX_HPP__

#include <pthread.h>

namespace hpsf
{
    class MutexLock
    {
        public:
            MutexLock()
            {
                pthread_mutex_init(&mutex_,NULL);
            }

            ~MutexLock()
            {
                pthread_mutex_destroy(&mutex_);
            }

            void lock()
            {
                pthread_mutex_lock(&mutex_);
            }

            void unlock()
            {
                pthread_mutex_unlock(&mutex_);
            }

            pthread_mutex_t* getPthreadMutex()
            {
                return &mutex_;
            }

        private:
            pthread_mutex_t mutex_;            
    };

    class MutexLockGuard
    {
        public:
            MutexLockGuard(MutexLock& mutex)
            :mutex_(mutex)
            {
                mutex_.lock();
            }
            ~MutexLockGuard()
            {
                mutex_.unlock();
            }
        private:
            MutexLock& mutex_;
    };
} // namespace hpsf


#endif