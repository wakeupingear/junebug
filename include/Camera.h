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
        Camera(Vec2<float> pos, Vec2<int> size);
        Camera(Vec2<int> pos, Vec2<int> size);
        Camera(Vec2<float> pos, Vec2<int> size, Vec2<float> screenPos, Vec2<int> screenSize);
        Camera(Vec2<int> pos, Vec2<int> size, Vec2<int> screenPos, Vec2<int> screenSize);
        ~Camera();

        Vec2<float> pos;
        Vec2<int> size;

        Vec2<float> screenPos;
        Vec2<int> screenSize;

    private:
        SDL_Texture *Render(SDL_Renderer *renderer);

        SDL_Texture *renderTex = nullptr;

        friend class JGame;
    };
}
