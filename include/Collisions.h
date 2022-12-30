#pragma once
#ifndef NAMESPACES
#define NAMESPACES
#endif

namespace junebug
{
    enum class CollType
    {
        None,
        Circle,
        Box,
        Tileset
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