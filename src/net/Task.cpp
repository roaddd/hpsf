#include "base/Task.hpp"
#include "base/IRun.hpp"

namespace hpsf
{
    Task::Task()
    :func0_(nullptr),
    func1_(nullptr)
    {
        
    }
    Task::Task(IRun0* func)
    :func0_(func),func1_(nullptr),func2_(nullptr),param_(nullptr)
    { }

    Task::Task(IRun1* func,const std::string& str,void* param)
    :func0_(nullptr),func1_(func),func2_(nullptr),str_(str),param_(param)
    { }

    Task::Task(IRun2* func,const std::string& str,std::any& any)
    :func0_(nullptr),func1_(nullptr),func2_(func),str_(str),any_(any)
    { }

    void Task::doTask()
    {
        if(func0_)func0_->run();
        if(func1_)func1_->run(str_,param_);
        if(func2_)func2_->run(str_,any_);
    }
} // namespace hpsf
