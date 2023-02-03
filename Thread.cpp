#include "Thread.hpp"
#include "IRun.hpp"
#include <sys/syscall.h>
#include <unistd.h>
#include "CurrentThread.hpp"

namespace hpsf
{
    __thread pid_t CurrentThread::t_cachedTid=0;

    void* globalRun(void* arg)
    {
        static_cast<Thread*>(arg)->run();
    }

    void CurrentThread::cacheTid()
    {
        if(t_cachedTid==0)
        {
            t_cachedTid=static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }

    Thread::Thread(IRun0* pRun)
    :run_(pRun),tid_(0)
    { }

    void Thread::start()
    {
        pthread_t t;
        pthread_create(&t,NULL,globalRun,this);
    }

    void Thread::run()
    {
        run_->run();
    }

    pid_t Thread::gettid()
    {
        return CurrentThread::tid();
    }
} // namespace hpsf
