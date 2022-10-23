#pragma once

#ifndef NAMESPACES
#define NAMESPACES
#endif

#include <iostream>

namespace Junebug
{
    // Print a message to the console
    // \param ...args Any number of arguments to print. Types must support << operator
    template <typename... T>
    void print(T... args)
    {
        ((std::cout << args << ' '), ...) << std::endl;
    };
}
