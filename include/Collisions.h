#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

#include "MathLib.h"

namespace junebug
{
    enum class CollType
    {
        None = -1,
        Circle = 0,
        Box = 1,
        Tileset = 2,
        Polygon = 3
    };

    enum class CollSide
    {
        None = -1,
        Left = 0,
        TopLeft = 1,
        Top = 2,
        TopRight = 3,
        Right = 4,
        BottomRight = 5,
        Bottom = 6,
        BottomLeft = 7
    };

    inline CollSide FlipCollSide(CollSide side)
    {
        if (side == CollSide::None)
            return CollSide::None;
        return (CollSide)(((int)side + 4) % 8);
    }

    inline CollSide VecCollSide(Vec2<float> vec)
    {
        return (CollSide)((int)(Atan2(vec.x, vec.y) / Pi * 4 + 8) % 8);
    }
};