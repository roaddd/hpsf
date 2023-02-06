#ifndef __TIMERID_HPP__
#define __TIMERID_HPP__

#include <stdint.h>

namespace  hpsf
{
    class Timer;
    class TimerId
    {
        public:
            TimerId(Timer* timer,int64_t seq)
            : timer_(timer),
            sequence_(seq)
            {

            }

            friend class TimerQueue;
        private:
            Timer* timer_;
            int64_t sequence_;
    };
} // namespace  hpsf


#endif