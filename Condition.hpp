#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include "Mutex.hpp"

namespace hpsf
{
    class Condition
    {
        public:
            Condition(MutexLock& mutex)
            :mutex_(mutex)
            {
                pthread_cond_init(&cond_,NULL);
            }

            ~Condition()
            {
                pthread_cond_destroy(&cond_);
            }

            void wait()
            {
                pthread_cond_wait(&cond_,mutex_.getPthreadMutex());
            }

            void notify()
            {
                pthread_cond_signal(&cond_);
            }

            void notifyAll()
            {
                pthread_cond_broadcast(&cond_);
            }
        private:
            MutexLock& mutex_;
            pthread_cond_t cond_;            
    };
} // namespace hpsf


#endif