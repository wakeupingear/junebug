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
        Camera(Vec2<float> pos, Vec2<float> size);
        Camera(Vec2<float> pos, Vec2<int> size);
        Camera(Vec2<int> pos, Vec2<int> size);

        Camera(Vec2<float> pos, Vec2<float> size, Vec2<float> screenPos, Vec2<float> screenSize);
        Camera(Vec2<int> pos, Vec2<int> size, Vec2<int> screenPos, Vec2<int> screenSize);
        ~Camera();

        Vec2<float> pos, _calcPos;
        Vec2<bool> fractionalPos{false, false};
        Vec2<float> size, _calcSize;
        Vec2<bool> fractionalSize{false, false};

        Vec2<float> screenPos, _calcScreenPos;
        Vec2<bool> fractionalScreenPos{false, false};
        Vec2<float> screenSize, _calcScreenSize;
        Vec2<bool> fractionalScreenSize{false, false};

    protected:
        friend class JGame;
        void _UpdateCoordinates();

    private:
        SDL_Texture *Render(SDL_Renderer *renderer);

        SDL_Texture *renderTex = nullptr;

        void _UpdateCoordinate(Vec2<float> &vec, Vec2<float> &outVec, Vec2<bool> &fractional, float w, float h);
        void _GuessFractional(Vec2<float> &vec, Vec2<bool> &outVec);
    };
}
