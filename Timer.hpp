#ifndef __TIMER_HPP__
#define __TIMER_HPP__

#include "Timestamp.hpp"
#include "IRun.hpp"
#include <cstddef> 

namespace hpsf
{
    class Timer
    {
        public:
            Timer(Timestamp stamp,IRun* pRun,double interval)
                :stamp_(stamp),id_(stamp),pRun_(pRun),interval_(interval)
            {
                
            }

            void run()
            {
                pRun_->run(NULL);
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
                return id_.getVal();
            }

            Timestamp getStamp()
            {
                return stamp_;
            }
        private:
            Timestamp stamp_;
            Timestamp id_;
            IRun* pRun_;
            double interval_;  
                    
    };
} // namespace hpsf



#endif