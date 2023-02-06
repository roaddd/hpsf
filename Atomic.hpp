#ifndef __ATOMIC_HPP__
#define __ATOMIC_HPP__

#include <stdint.h>

namespace hpsf
{
    template<typename T>
    class AtomicIntegerT
    {
        public:
            AtomicIntegerT()
            :value_(0)
            {

            }

            T getAndAdd(T x)
            {
                return __atomic_fetch_add(&value_,x,__ATOMIC_SEQ_CST);
            }

            T addAndGet(T x)
            {
                return getAndAdd(x)+x;
            }

            T incrementAndGet()
            {
                return addAndGet(1);
            }
        private:
            volatile T value_;
    };

    typedef AtomicIntegerT<int64_t> AtomicInt64;
} // namespace hpsf


#endif