#ifndef __IRUN_HPP__
#define __IRUN_HPP__

namespace hpsf
{
    class IRun
    {
        public:
            virtual void run(void* arg)=0;
    };
}

#endif