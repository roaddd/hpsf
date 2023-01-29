#ifndef _BUFFER_HPP__
#define _BUFFER_HPP__

#include <string>

namespace hpsf
{
    class Buffer
    {
        public:
            Buffer();
            ~Buffer();
            
            void append(const std::string& data);

            const char* peek();
            int readableBytes();
            void retrive(int len);

            std::string retrieveAsString(size_t len);
            std::string retrieveAllAsString();
        private:
            std::string buffer_;
    };
} // namespace hpsf


#endif