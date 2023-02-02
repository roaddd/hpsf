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
    class TimerQueue:public IChannelCallBack
    {
        public:
            TimerQueue(EventLoop* loop);
            ~TimerQueue();
            int addTimer(IRun* pRun,Timestamp when,double interval);
            void cancel(int timerid);
            virtual void handleRead();
            virtual void handleWrite();

            class AddTimerWrapper : public IRun
            {
                public:
                    AddTimerWrapper(TimerQueue* pQueue)
                        :_pQueue(pQueue){};
                    virtual void run(void* param)
                    {
                        _pQueue->doAddTimer(param);
                    };
                private:
                    TimerQueue* _pQueue;
            };

            class CancelTimerWrapper : public IRun
            {
                public:
                    CancelTimerWrapper(TimerQueue* pQueue)
                        :_pQueue(pQueue){};
                    virtual void run(void* param)
                    {
                        _pQueue->doCancelTimer(param);
                    }
                private:
                    TimerQueue* _pQueue;
            };

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
            AddTimerWrapper pAddTimerWrapper_;
            CancelTimerWrapper pCancelTimerWrapper_;
    };
} // namespace hpsf


#endif