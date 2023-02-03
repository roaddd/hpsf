#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <string>

namespace hpsf
{
    class IRun0;
    class IRun1;
    class Task
    {
        public:
            Task(IRun0* func);
            Task(IRun1* func,const std::string& str,void* param);
            void doTask();
        private:
            IRun0* func0_;
            IRun1* func1_;
            std::string str_;
            void* param_;
    };
} // namespace hpsf


#endif