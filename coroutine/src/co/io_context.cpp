#include "co/Task.hpp"
#include "co/io_context.hpp"
#include "Utils.hpp"

#include <sys/epoll.h>

#define can_read EPOLLIN
#define can_write EPOLLOUT

namespace co
{
    io_context::io_context()
        : tid(-1),
          is_active(true),
          queue_condition(mutex_),
          epoll_(new Epoll())
    {
    }

    void io_context::co_spawn(Task<void> &&task,int fd)
    {
        std::coroutine_handle<> handle=task.get_handle();
        task.detach();
        if(fd==-1){
            queue_.push(handle);
        }
        else{
            update(handle,fd);
        }
    }

    void io_context::join()
    {
        thread_.join();
    }

    void io_context::start()
    {
        thread_ = std::thread([this]()
                              {
            //init io_context
            this->run(); });
    }

    void io_context::run()
    {
        while(queue_.size()){
            auto t_handle=queue_.front();
            queue_.pop();
            t_handle.resume();
        }
        while (is_active.load(std::memory_order_relaxed) || !queue_.empty())
        {
            vector<Channel*> activates_;
            epoll_->poll(&activates_);
            for(int i=0;i<activates_.size();i++){
                activates_[i]->resume();
            }
        }
    }

    void io_context::update(int fd){
        
    }

    void io_context::update(std::coroutine_handle<> handle,int fd)
    {
        channels_[fd]=std::move(Channel(handle,fd));//调用Channel()???
        channels_[fd].setEvents(can_read);
        //TODO：use shared_ptr
        epoll_->update(&channels_[fd]);
    }
} // namespace co
