#ifndef __TIMERQUEUE_HPP__
#define __TIMERQUEUE_HPP__

#include <set>
#include "Timestamp.hpp"
#include <vector>
#include "IRun.hpp"
#include "Channel.hpp"

namespace hpsf
{
    class EventLoop;
    class Timer;
    class TimerQueue:public IChannelCallBack,public IRun1
    {
        public:
            TimerQueue(EventLoop* loop);
            ~TimerQueue();
            int addTimer(IRun0* pRun,Timestamp when,double interval);
            void cancel(int timerid);
            virtual void handleRead();
            virtual void handleWrite();

            //IRun1
            virtual void run(const std::string& str,void* timer);

        private:
            typedef std::pair<Timestamp,Timer*> Entry;
            typedef std::set<Entry> TimerList;

            void doAddTimer(void* parm);
            void doCancelTimer(void* parm);
            bool insert(Timer* pTimer);
            void resetTimerfd(int timerfd,Timestamp stamp);
            void readTimerfd(int timerfd,Timestamp stamp);
            void reset(std::vector<Entry>& expired,Timestamp now);
            std::vector<Entry> getExpired(Timestamp now);
            
            int timerfd_;
            EventLoop* loop_;
            Channel timerfdChannel_;
            TimerList timers_;
    };
} // namespace hpsf


#endif