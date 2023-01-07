#pragma once

#include "SDL2/SDL.h"
#include <ostream>

struct Color : SDL_Color
{
    Color() = default;
    Color(const Color &) = default;
    Color(Color &&) = default;
    Color &operator=(const Color &) = default;
    Color &operator=(Color &&) = default;
    ~Color() = default;

    Color(const SDL_Color &color) : SDL_Color(color) {}
    Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) : SDL_Color{r, g, b, a} {}

    Color &operator=(const SDL_Color &color)
    {
        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
        return *this;
    }

    bool operator==(const Color &color) const
    {
        return r == color.r && g == color.g && b == color.b && a == color.a;
    }

    bool operator!=(const Color &color) const
    {
        return !(*this == color);
    }

    [[nodiscard]] friend Color operator*(const Color &vec, float scalar)
    {
        return Color(vec.r * scalar, vec.g * scalar, vec.b * scalar, vec.a * scalar);
    }

    friend std::ostream &operator<<(std::ostream &os, const Color &color)
    {
        os << "Color(" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << ", " << (int)color.a << ")";
        return os;
    }

    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Transparent;
};