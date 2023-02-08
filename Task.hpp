#ifndef __TASK_HPP__
#define __TASK_HPP__

#include <string>
#include <any>

namespace hpsf
{
    class IRun0;
    class IRun1;
    class IRun2;
    class Task
    {
        public:
            Task();
            Task(IRun0* func);
            Task(IRun1* func,const std::string& str,void* param);
            Task(IRun2* func,const std::string& str,std::any& any);
            void doTask();
            bool valid(){return func0_ || func1_;}
        private:
            IRun0* func0_;
            IRun1* func1_;
            std::string str_;
            void* param_;
            IRun2* func2_;
            std::any any_;
    };
} // namespace hpsf


#endif