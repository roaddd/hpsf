#ifndef __ECHOSERVER_HPP__
#define __ECHOSERVER_HPP__

#include "IMuduoUser.hpp"
#include "TcpServer.hpp"
#include "ThreadPool.hpp"

namespace hpsf
{
    
    class EchoServer:public IMuduoUser,public IRun1
    {
        public:
            /**
             * @brief 
             * nums1:nums of work threads;
             * nums2:nums of loop threads;
             * 
             */
            EchoServer(EventLoop* loop,int nums1,int nums2);
            ~EchoServer();

            virtual void onConnection(TcpConnection* connection);
            virtual void onMessage(TcpConnection* connection,Buffer* buffer);
            virtual void onWriteComplate(TcpConnection* connection);

            //IRun1
            virtual void run(const std::string& str,void* param);

            void start();
            int fib(int x);

        private:
            EventLoop* loop_;
            TcpServer tcpServer;
            ThreadPool threadPool_;
            int numThreads_;
    };
    
} // namespace hpsf


#endif