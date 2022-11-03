#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"

#include "SDL2/SDL.h"
#include <iostream>

namespace junebug
{
    // Print a message to the console
    /// @param ...args Any number of arguments to print. Types must support << operator
    template <typename... T>
    inline void print(T... args)
    {
        ((std::cout << args << ' '), ...) << std::endl;
    };

    // Get a render texture with the given size.
    // You MUST store the return value of this function as it may contain a new texture in the case that the old one was invalid
    /// @param size The size of the render texture
    /// @param renderer The renderer to create the texture with
    /// @param texture An optional pointer to the render texture
    /// @param enableForRenderer Whether or not to enable the texture for the renderer
    /// @return A pointer to the render texture
    SDL_Texture *GetRenderTexture(Vec2<int> size, SDL_Renderer *renderer, SDL_Texture *texture = nullptr, bool enableForRenderer = false);

#define Twerp_Undefined -127.0f
#define Twerp_Back_DefaultBounciness 1.5f
    enum TwerpType
    {
        TWERP_LINEAR,
        TWERP_INOUT_BACK,
        TWERP_IN_BACK,
        TWERP_OUT_BACK,
        TWERP_INOUT_BOUNCE,
        TWERP_OUT_BOUNCE,
        TWERP_IN_BOUNCE,
        TWERP_INOUT_CIRCLE,
        TWERP_OUT_CIRCLE,
        TWERP_IN_CIRCLE,
        TWERP_INOUT_CUBIC,
        TWERP_OUT_CUBIC,
        TWERP_IN_CUBIC,
        TWERP_INOUT_ELASTIC,
        TWERP_OUT_ELASTIC,
        TWERP_IN_ELASTIC,
        TWERP_INOUT_EXPO,
        TWERP_OUT_EXPO,
        TWERP_IN_EXPO,
        TWERP_INOUT_QUAD,
        TWERP_OUT_QUAD,
        TWERP_IN_QUAD,
        TWERP_INOUT_QUART,
        TWERP_OUT_QUART,
        TWERP_IN_QUART,
        TWERP_INOUT_QUINT,
        TWERP_OUT_QUINT,
        TWERP_IN_QUINT,
        TWERP_INOUT_SINE,
        TWERP_OUT_SINE,
        TWERP_IN_SINE,
        TWERP_COUNT
    };

    // Fancy lerp function with various curve types
    // Credit to PixelatedPope
    /// @param type The type of curve to use
    /// @param start The start value
    /// @param end The end value
    /// @param pos The position on the curve
    /// @param looped OPTIONAL Whether or not the curve is looped
    /// @return The value at the given position on the curve
    float Twerp(TwerpType _type, float _start, float _end, float _pos, bool _looped = false, float _opt1 = Twerp_Undefined, float _opt2 = Twerp_Undefined);
}
