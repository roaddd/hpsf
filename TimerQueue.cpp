#include "TimerQueue.hpp"
#include "EventLoop.hpp"
#include "Timer.hpp"
#include <sys/timerfd.h>
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <strings.h>
#include <algorithm>
#include <iterator>
#include "Task.hpp"


namespace hpsf
{

    struct timespec howMuchTimeFromNow(Timestamp stamp)
    {
        int64_t microseconds=stamp.microSecondsSinceEpoch()-Timestamp::now().microSecondsSinceEpoch();
        if(microseconds<100)microseconds=100;
        struct timespec ts;
        ts.tv_sec=static_cast<time_t>(microseconds/Timestamp::kMicroSecondsPerSecond);
        ts.tv_nsec=static_cast<long>((microseconds%Timestamp::kMicroSecondsPerSecond)*1000);
        return ts;
    }
    
    int createTimerfd()
    {
        int timerfd=::timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC|TFD_NONBLOCK);
        if(timerfd<0)
        {
            std::cout<<"createTimerfd error"<<std::endl;
            exit(1);
        }
        return timerfd;
    }

    TimerQueue::TimerQueue(EventLoop* loop)
        :loop_(loop),timerfd_(createTimerfd()),
        timerfdChannel_(loop,timerfd_),
        timers_()
    {
        timerfdChannel_.setCallBack(this);
        timerfdChannel_.enableReading();
        std::cout<<"TimerQueue created"<<std::endl;
    }

    TimerQueue::~TimerQueue()
    {
        ::close(timerfd_);
    }

    int TimerQueue::addTimer(IRun0* pRun,Timestamp when,double interval)
    {
        Timer* pTimer=new Timer(when,pRun,interval);
        Task task(this,"addTimer",pTimer);
        loop_->runInLoop(task);
        return pTimer->getId();
    }

    void TimerQueue::run(const std::string& str,void* param)
    {
        if(str=="addTimer")
        {
            std::cout<<"addTimer"<<std::endl;
            doAddTimer(param);
        }
        else
            doCancelTimer(param);
    }

    void TimerQueue::doAddTimer(void* parm)
    {
        Timer* pTimer=static_cast<Timer*>(parm);
        bool earliestChanged = insert(pTimer);
        if(earliestChanged)
        {
            resetTimerfd(timerfd_,pTimer->getStamp());
        }
    }

    void TimerQueue::cancel(int timerid)
    {

    }

    void TimerQueue::doCancelTimer(void* parm)
    {
        Timer* pTimer=static_cast<Timer*>(parm);
        for(auto it=timers_.begin();it!=timers_.end();it++)
        {
            if(it->second==pTimer)
            {
                timers_.erase(it);
                break;
            }
        }
    }

    bool TimerQueue::insert(Timer* pTimer)
    {
        bool earliestChanged=false;
        Timestamp when=pTimer->getStamp();
        auto it=timers_.begin();
        if(it==timers_.end() || when<it->first)
        {
            earliestChanged=true;
        }
        auto result=timers_.insert(Entry(when,pTimer));
        if(!result.second)
        {
            std::cout<<"insert error"<<std::endl;
        }
        return earliestChanged;
    }

    void TimerQueue::resetTimerfd(int timerfd,Timestamp stamp)
    {
        struct itimerspec newValue,oldValue;
        bzero(&newValue,sizeof(newValue));
        bzero(&oldValue,sizeof(oldValue));
        newValue.it_value=howMuchTimeFromNow(stamp);
        int ret=::timerfd_settime(timerfd,0,&newValue,&oldValue);
        if(ret)
        {
            std::cout<<"timerfd_settime error"<<std::endl;
        }
    }

    void cancel(int timerId)
    {
        //TODO
    }

    void TimerQueue::handleRead()
    {
        Timestamp now(Timestamp::now());
        readTimerfd(timerfd_,now);
        std::vector<Entry> expired=getExpired(now);
        for(auto it=expired.begin();it!=expired.end();it++)
        {
            it->second->run();
        }
        reset(expired,now);
    }

    void TimerQueue::reset(std::vector<Entry>& expired,Timestamp now)
    {
        for(auto it=expired.begin();it!=expired.end();it++)
        {
            if(it->second->isReapt())
            {
                it->second->moveToNext();
                insert(it->second);
            }
        }
        Timestamp nextExpire;
        if(!timers_.empty())
        {
            nextExpire=timers_.begin()->first;
        }
        if(nextExpire.valid())
        {
            resetTimerfd(timerfd_,nextExpire);
        }
    }

    void TimerQueue::readTimerfd(int timerfd,Timestamp stamp)
    {
        uint64_t howmany;
        ssize_t n=::read(timerfd_,&howmany,sizeof(howmany));
        if(n!=sizeof(ssize_t))
        {
            std::cout<<"TimerQueue::readTimefd error"<<std::endl;
        }
    }

    std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
    {
        std::vector<Entry> expired;
        Entry sentry(now,reinterpret_cast<Timer*>(UINTPTR_MAX));
        auto end=timers_.lower_bound(sentry);
        std::copy(timers_.begin(),end,back_inserter(expired));
        timers_.erase(timers_.begin(),end);
        return expired;
    }

    void TimerQueue::handleWrite()
    {

    }

}