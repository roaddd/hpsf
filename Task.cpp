#include "Task.hpp"
#include "IRun.hpp"

namespace hpsf
{
    Task::Task(IRun0* func)
    :func0_(func),func1_(nullptr),param_(nullptr)
    { }

    Task::Task(IRun1* func,const std::string& str,void* param)
    :func0_(nullptr),func1_(func),str_(str),param_(param)
    { }

    void Task::doTask()
    {
        if(func0_)func0_->run();
        if(func1_)func1_->run(str_,param_);
    }
} // namespace hpsf
