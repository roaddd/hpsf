#ifndef __THREAD_HPP__
#define __THREAD_HPP__

#include "IRun.hpp"

#include <pthread.h>


namespace hpsf
{
    class Thread
    {
        public:
            Thread(IRun0* pRun);
            void start();
            void run();
            pid_t gettid();
            int join();
        private:
            IRun0* run_;
            pid_t tid_;
            pthread_t pthreadId_;
    };
} // namespace hpsf


#endif