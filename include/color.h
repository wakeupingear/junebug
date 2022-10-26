#pragma once

class Color
{
public:
    Color();
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);

    unsigned char r, g, b, a;

    static const Color White;
    static const Color Black;
};