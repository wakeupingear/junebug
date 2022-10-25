#include "Color.h"

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : r(r), g(g), b(b), a(a)
{
}

const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(0, 0, 0);