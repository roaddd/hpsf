#include "Buffer.hpp"

namespace hpsf
{
    Buffer::Buffer()
    {

    }

    Buffer::~Buffer()
    {
        
    }

    const char* Buffer::peek()
    {
        return buffer_.c_str();
    }

    int Buffer::readableBytes()
    {
        return buffer_.size();
    }

    void Buffer::retrive(int len)
    {
        buffer_=buffer_.substr(len)
;    }

    void Buffer::append(const std::string& data)
    {
        buffer_.append(data);
    }

    std::string Buffer::retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }

    std::string Buffer::retrieveAsString(size_t len)
    {
        std::string res(peek(),len);
        retrive(len);
        return res;
    }

}
