#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include "Timestamp.hpp"
#include "IRun.hpp"
#include "Atomic.hpp"

#include <cstddef> 
#include <stdint.h>

namespace hpsf
{
    class Timer
    {
        public:
            Timer(Timestamp stamp,IRun0* pRun,double interval)
                :stamp_(stamp),pRun_(pRun),interval_(interval),
                sequence_(s_numCreated_.incrementAndGet())
            {
            }

            void run()
            {
                pRun_->run();
            }

            bool isReapt()
            {
                return interval_>0.0;
            }

            void moveToNext()
            {
                stamp_=Timestamp::nowAfter(interval_);
            }

            int getId()
            {
                return sequence_;
            }

            Timestamp getStamp()
            {
                return stamp_;
            }
        private:
            Timestamp stamp_;
            int64_t sequence_;
            IRun0* pRun_;
            double interval_;  
                    
            static AtomicInt64 s_numCreated_;
    };

    AtomicInt64 Timer::s_numCreated_;
} // namespace hpsf



#endif