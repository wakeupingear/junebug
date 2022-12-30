#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

namespace junebug
{
    enum class CollType
    {
        None = 0,
        Circle = 1,
        Box = 2,
        Tileset = 3
    };

    enum class CollSide
    {
        None,
        Top,
        Bottom,
        Left,
        Right
    };

    inline CollSide FlipCollSide(CollSide side)
    {
        switch (side)
        {
        case CollSide::Top:
            return CollSide::Bottom;
        case CollSide::Bottom:
            return CollSide::Top;
        case CollSide::Left:
            return CollSide::Right;
        case CollSide::Right:
            return CollSide::Left;
        default:
            return CollSide::None;
        }
    }
};