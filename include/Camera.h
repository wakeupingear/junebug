#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"

#include "SDL2/SDL.h"

namespace junebug
{
    class Camera
    {
    public:
        Camera();
        Camera(Vec2 pos, Vec2 size);
        Camera(Vec2 pos, Vec2 size, Vec2 screenPos, Vec2 screenSize);
        ~Camera();

        Vec2 pos;
        Vec2 size;

        Vec2 screenPos;
        Vec2 screenSize;

    private:
        SDL_Texture *Render(SDL_Renderer *renderer);

        SDL_Texture *renderTex = nullptr;

        friend class JGame;
    };
}
