#ifndef __TIMESTAMP_HPP__
#define __TIMESTAMP_HPP__

#include <stdint.h> 

namespace hpsf
{
    class Timestamp
    {
        public:
            Timestamp();
            ~Timestamp();

            explicit Timestamp(int64_t microSecondsSinceEpochArg);
            
            int getVal();
            int64_t microSecondsSinceEpoch(){return microSecondsSinceEpoch_;}
            bool valid(){return microSecondsSinceEpoch_>0;}

            static const int kMicroSecondsPerSecond = 1000 * 1000;
            static Timestamp now();
            static Timestamp nowAfter(double s);
            static double nowMicroSeconds();
        private:
            int64_t microSecondsSinceEpoch_;
    };

    inline bool operator==(Timestamp t1,Timestamp t2)
    {
        return t1.microSecondsSinceEpoch()==t2.microSecondsSinceEpoch();
    }

    inline bool operator<(Timestamp t1,Timestamp t2)
    {
        return t1.microSecondsSinceEpoch()<t2.microSecondsSinceEpoch();
    }

} // namespace hpsf


#endif