#ifndef __CHANNEL_CO_HPP__
#define __CHANNEL_CO_HPP__

#include <coroutine>

namespace co
{
    class Channel
    {
    public:
        Channel();

        Channel(std::coroutine_handle<> handle,int fd);

        Channel(Channel&& other){
            fd_=other.fd_;
            events_=other.events_;
            revents_=other.revents_;
            index_=other.index_;
            handle_=other.handle_;
            other.fd_=-1;
            other.index_=-1;
            other.events_=-1;
            other.revents_=-1;
            other.handle_=std::noop_coroutine();
        }

        Channel& operator=(Channel&& other){
            fd_=other.fd_;
            events_=other.events_;
            revents_=other.revents_;
            index_=other.index_;
            handle_=other.handle_;
            other.fd_=-1;
            other.index_=-1;
            other.events_=-1;
            other.revents_=-1;
            other.handle_=std::noop_coroutine();
            return *this;
        }

        ~Channel();
        void handleEvent();
        void setEvents(int event){events_=event;}
        void setRevents(int revents){revents_=revents;}
        void setIndex(int index);
        int getfd();
        int getIndex() const;
        int revents() const;
        int getEvents() const;
        int getRevents(){return revents_;}
        void enableReading();
        void disableReading();
        void enableWriting();
        void disableWriting();
        bool isWriting();

        /**
         * @brief resume handle
         * 
         */
        void resume(){handle_.resume();}
    private:
        void update();
        int fd_;
        int events_, revents_;
        int index_;
        std::coroutine_handle<> handle_;
        //io_context* co;
    };
} // namespace co

#endif