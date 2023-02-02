#include "Timestamp.hpp"
#include <sys/time.h>
#include <stddef.h>

namespace hpsf
{
    Timestamp::Timestamp():microSecondsSinceEpoch_(0)
    {

    }

    Timestamp::~Timestamp()
    {

    }

    Timestamp::Timestamp(int64_t microSecondsSinceEpochArg)
    : microSecondsSinceEpoch_(microSecondsSinceEpochArg)
    {

    }    

    int Timestamp::getVal()
    {
        return static_cast<int>(microSecondsSinceEpoch_);
    }

    Timestamp Timestamp::now()
    {
        return Timestamp(Timestamp::nowMicroSeconds());
    }

    double Timestamp::nowMicroSeconds()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        int64_t seconds=tv.tv_sec;
        return seconds*kMicroSecondsPerSecond+tv.tv_usec;
    }

    Timestamp Timestamp::nowAfter(double s)
    {
        return Timestamp(Timestamp::nowMicroSeconds()+kMicroSecondsPerSecond*s);
    }
} // namespace hpsf
