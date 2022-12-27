#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"
#include "Compatability.h"
#include "Rendering.h"

#include "SDL2/SDL.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>

namespace junebug
{
    // To avoid circular dependencies, defining this later lets us access singleton behaviors in Game from these namespace-declared functions
    // This is a horrendous anti-pattern but was necessary for debugging purposes
    enum __GameFunctions__
    {
        SkipPrintThisFrame,
        RaiseWindow
    };
    void __CallGameFunction__(__GameFunctions__ func);

    // Get the current time in milliseconds
    /// @return The current time in milliseconds
    std::string PrintTime();

    inline void __PostPrint__()
    {
#ifndef NDEBUG
        __CallGameFunction__(__GameFunctions__::SkipPrintThisFrame);
#endif
    }

    // Print a message to the console
    /// @param ...args Any number of arguments to print. Types must support << operator
    template <typename... T>
    inline void Print(T... args)
    {
        ((std::cout << args << ' '), ...) << std::endl;
        __PostPrint__();
    };
    template <typename... T>
    inline void print(T... args)
    {
        Print(args...);
    };

    template <typename... T>
    inline void PrintNoSpaces(T... args)
    {
        ((std::cout << args), ...) << std::endl;
        __PostPrint__();
    };
    template <typename... T>
    inline void printNoSpaces(T... args)
    {
        PrintNoSpaces(args...);
    };

    template <typename... T>
    inline void PrintHalt(T... args)
    {
        ((std::cout << args << ' '), ...);
        std::cin.get();
        std::cout << std::endl;
        __CallGameFunction__(__GameFunctions__::RaiseWindow);
        __PostPrint__();
    }
    template <typename... T>
    inline void printHalt(T... args)
    {
        PrintHalt(args...);
    }

    static std::ofstream __logStream__("log.txt", std::ofstream::out);

    // Log a message
    /// @param ...args Any number of arguments to print. Types must support << operator
    template <typename... T>
    void Log(T... args)
    {
        __logStream__ << "[" << PrintTime() << "] ";
        ((__logStream__ << args << ' '), ...) << std::endl;
    };
    template <typename... T>
    void log(T... args)
    {
        Log(args...);
    };

    // Print and log a message
    /// @param ...args Any number of arguments to print. Types must support << operator
    template <typename... T>
    inline void PrintLog(T... args)
    {
        Print(args...);
        Log(args...);
    };
    template <typename... T>
    inline void printLog(T... args)
    {
        PrintLog(args...);
    };

    // Get a render texture with the given size.
    // You MUST store the return value of this function as it may contain a new texture in the case that the old one was invalid
    /// @param size The size of the render texture
    /// @param renderer The renderer to create the texture with
    /// @param texture An optional pointer to the render texture
    /// @param enableForRenderer Whether or not to enable the texture for the renderer
    /// @return A pointer to the render texture
    SDL_Texture *GetRenderTexture(Vec2<int> size, SDL_Renderer *renderer, SDL_Texture *texture = nullptr, bool enableForRenderer = false);

    // Check if a string ends with another string
    /// @param str The string to check
    /// @param ending The ending to check for
    /// @return Whether or not the string ends with the ending
    bool StringEndsWith(const std::string &str, const std::string &ending);

    // Check if a string consists of only whitespace
    /// @param str The string to check
    /// @return Whether or not the string is whitespace
    bool IsWhitespace(const std::string &str);

    template <typename T>
    inline float RoundDec(T val, int n)
    {
        int div = std::pow(10, n);
        T value = (int)(val * div + .5);
        return (T)value / div;
    }

    template <typename T>
    inline std::string RoundDecStr(T val, int n)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << val;
        return out.str();
    }

    // Get the name of a file from a path
    /// @param path The path to get the file name from
    /// @return The file name
    std::string GetFileName(const std::string &path);
    // Trim the file extension from a file name
    /// @param fileName The file name to trim
    /// @return The file name without the extension
    std::string TrimFileExtension(const std::string &fileName);

    // Generate a UUID v4
    /// @return string The UUID
    std::string UUID();
};
