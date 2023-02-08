#ifndef __IRUN_HPP__
#define __IRUN_HPP__

#include <string>
#include <any>

namespace hpsf
{
    class IRun0
    {
        public:
            virtual void run()=0;
    };

    class IRun1
    {
        public:
            virtual void run(const std::string& str,void* param)=0;
    };

    class IRun2
    {
        public:
            virtual void run(const std::string& str,std::any& any)=0;
    };
}

#endif