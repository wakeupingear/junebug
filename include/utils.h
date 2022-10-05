#pragma once

#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <iostream>

namespace Junebug
{
    template <typename... T>
    void print(T... args)
    {
        ((std::cout << args << ' '), ...) << std::endl;
    };
}
