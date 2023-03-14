#ifndef __CURRENTTHREAD_HPP__
#define __CURRENTTHREAD_HPP__
#include <sys/types.h>

namespace hpsf
{
    namespace CurrentThread
    {
        extern __thread pid_t t_cachedTid;

        void cacheTid();

        inline pid_t tid()
        {
            if(__builtin_expect(t_cachedTid==0,0))
            {
                cacheTid();
            }
            return t_cachedTid;
        }
        
    } // namespace CurrentThread

} // namespace hpsf

#endif