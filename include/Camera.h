#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"

namespace junebug
{
    class Camera
    {
    public:
        Camera();
        Camera(Vector2 pos, Vector2 size);
        Camera(Vector2 pos, Vector2 size, Vector2 screenPos, Vector2 screenSize);
        ~Camera();

        Vector2 pos;
        Vector2 size;

        Vector2 screenPos;
        Vector2 screenSize;

    private:
        void Render();

        friend class JGame;
    };
}
