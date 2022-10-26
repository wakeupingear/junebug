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
    SDL_Texture *GetRenderTexture(Vector2 size, SDL_Renderer *renderer, SDL_Texture *texture = nullptr, bool enableForRenderer = false);
}
