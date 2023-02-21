#ifndef __TASKAWAITER_HPP__
#define __TASKAWAITER_HPP__

namespace co
{
    template<typename T>
    class TaskAwaiter
    {
        public: 
            bool await_ready() const{return false;}
            void await_suspend(std::coroutine_handle<> handle)
            {
                
            }
            void await_resume(){ }
        //private:
    };

} // namespace co

#endif