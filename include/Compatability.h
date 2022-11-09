#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <ctime>

namespace junebug
{
    inline std::tm __LocaltimeSafe__(std::time_t timer)
    {
        std::tm bt{};
#if defined(__unix__)
        localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
        localtime_s(&bt, &timer);
#else
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        bt = *std::localtime(&timer);
#endif
        return bt;
    }
};