#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

inline void PrintTime()
{
    using namespace std;
    using namespace std::chrono;

    auto now = system_clock::now();
    auto in_time_t = system_clock::to_time_t(now);

    // get number of milliseconds for the current second
    // (remainder after division into seconds)
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    cout << std::put_time(std::localtime(&in_time_t), "%T")
         << '.' << std::setfill('0') << std::setw(3) << ms.count();
}

inline void PrintThread()
{
    using namespace std;
    cout << " [Thread-" << gettid() << "] ";
}

template <typename... U>
inline void PrintIn(U... u)
{
    using namespace std;
    int i = 0;
    auto printer = [&i]<typename Arg>(Arg arg)
    {
        if (sizeof...(U) == ++i)
            cout << arg << endl;
        else
            cout << arg << " ";
    };
    (printer(u), ...);
}

#define debug(...)                                       \
    PrintTime();                                         \
    PrintThread();                                       \
    printf("(%s:%d) %s: ", __FILE__, __LINE__, __func__); \
    PrintIn(__VA_ARGS__);

#endif