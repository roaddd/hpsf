#include "base/Thread.hpp"
#include "base/IRun.hpp"
#include "base/CurrentThread.hpp"

#include <sys/syscall.h>
#include <unistd.h>
#include <iostream>

namespace hpsf
{
    __thread pid_t CurrentThread::t_cachedTid=0;

    void* globalRun(void* arg)
    {
        std::cout<<"new Thread created, thread id: "<<CurrentThread::tid()<<std::endl;
        static_cast<Thread*>(arg)->run();
        return NULL;
    }

    void CurrentThread::cacheTid()
    {
        if(t_cachedTid==0)
        {
            t_cachedTid=static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }

    Thread::Thread(IRun0* pRun)
    :run_(pRun),
    tid_(0),
    pthreadId_(0)
    {

    }

    void Thread::start()
    {
        pthread_create(&pthreadId_,NULL,globalRun,this);
    }

    void Thread::run()
    {
        run_->run();
    }

    pid_t Thread::gettid()
    {
        return CurrentThread::tid();
    }

    int Thread::join()
    {
        return pthread_join(pthreadId_,NULL);
    }
} // namespace hpsf
